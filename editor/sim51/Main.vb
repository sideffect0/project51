
Imports sim51.Functions

Public Class Main

    Public TextChange As Boolean = False
    Dim Compiled As Boolean = False
    Public CurFileName As String = "Untitled.asm"
    Public WorkDir As String

    'main menu

    Private Sub NewToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NewToolStripMenuItem.Click
        NewFile()
    End Sub

    Private Sub OpenToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenToolStripMenuItem.Click
        OpenFile()
    End Sub

    Private Sub SaveToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SaveToolStripMenuItem.Click
        SaveFile()
    End Sub

    Private Sub SaveAsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SaveAsToolStripMenuItem.Click
        SaveFile()
    End Sub

    Private Sub ExitToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExitToolStripMenuItem.Click
        If TextChange = True Then
            If MsgBox("Program not saved. Do you want to save it?", MsgBoxStyle.YesNo) = MsgBoxResult.Yes Then
                SaveFile()
            End If
        End If
        Me.Dispose()
    End Sub

    Private Sub UndoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles UndoToolStripMenuItem.Click
        TXT_Area.Undo()
    End Sub

    Private Sub RedoToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RedoToolStripMenuItem.Click
        TXT_Area.Redo()
    End Sub

    Private Sub CutToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CutToolStripMenuItem.Click
        TXT_Area.Cut()
    End Sub

    Private Sub CopyToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CopyToolStripMenuItem.Click
        TXT_Area.Copy()
    End Sub

    Private Sub PasteToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PasteToolStripMenuItem.Click
        TXT_Area.Paste()
    End Sub

    Private Sub SelectAllToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SelectAllToolStripMenuItem.Click
        TXT_Area.SelectAll()
    End Sub

    'context menu
    Private Sub CM_Cut_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CM_Cut.Click
        TXT_Area.Cut()
    End Sub

    Private Sub CM_Copy_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CM_Copy.Click
        TXT_Area.Copy()
    End Sub

    Private Sub CM_Paste_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CM_Paste.Click
        TXT_Area.Paste()
    End Sub

    Private Sub CM_Delete_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CM_Delete.Click
        TXT_Area.SelectedText = ""
    End Sub

    Private Sub CM_SelectAll_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CM_SelectAll.Click
        TXT_Area.SelectAll()
    End Sub

    'Toolbar buttons

    Private Sub NewToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles NewToolStripButton.Click
        NewFile()
    End Sub

    Private Sub OpenToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles OpenToolStripButton.Click
        OpenFile()
    End Sub

    Private Sub SaveToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SaveToolStripButton.Click
        SaveFile()
    End Sub

    Private Sub CutToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CutToolStripButton.Click
        TXT_Area.Cut()
    End Sub

    Private Sub CopyToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CopyToolStripButton.Click
        TXT_Area.Copy()
    End Sub

    Private Sub PasteToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles PasteToolStripButton.Click
        TXT_Area.Paste()
    End Sub

    Private Sub DeleteToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DeleteToolStripButton.Click
        TXT_Area.SelectedText = ""
    End Sub

    Private Sub UndoToolStripButton3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles UndoToolStripButton3.Click
        TXT_Area.Undo()
    End Sub

    Private Sub RedoToolStripButton4_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RedoToolStripButton4.Click
        TXT_Area.Redo()
    End Sub

    Private Sub CompileToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles CompileToolStripButton.Click
        Dim erPath, Err As String
        Try
            If TextChange = False And CurFileName <> "Untitled.asm" Then
                erPath = IO.Path.GetDirectoryName(CurFileName)
                erPath = IO.Path.Combine(erPath, IO.Path.GetFileNameWithoutExtension(CurFileName))
                erPath = erPath + ".err"

                Shell("sim51c.exe -q " + """" + CurFileName + """", AppWinStyle.Hide, True)

                Err = IO.File.ReadAllText(erPath)

                If Err = "" Then
                    MSG.Items.Add("Compilation success!")
                    MSG.SelectedIndex = MSG.Items.Count - 1
                    Compiled = True
                Else
                    MSG.Items.Add(Err)
                    MSG.SelectedIndex = MSG.Items.Count - 1
                    My.Computer.Audio.PlaySystemSound(Media.SystemSounds.Beep)
                End If
                Dim Path As String = IO.Path.GetDirectoryName(CurFileName)
                IO.File.Delete(Path + "\gmon.out")
            Else
                MsgBox("Please save the file before compiling.")
            End If

        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

    Private Sub RunToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RunToolStripButton.Click
        Dim oPath As String
        Try
            If Compiled = True Then
                oPath = IO.Path.GetDirectoryName(CurFileName)
                oPath = IO.Path.Combine(oPath, IO.Path.GetFileNameWithoutExtension(CurFileName))
                oPath = oPath + ".obj"

                Shell("sim51e.exe " + """" + oPath + """", AppWinStyle.Hide, True)

                ListMem()
                LoadReg()
                ListReg(Bank.Value)
                LoadPort()
                ListrPort()

                MSG.Items.Add("Program executed successfully.")
                MSG.SelectedIndex = MSG.Items.Count - 1
                Dim Path As String = IO.Path.GetDirectoryName(CurFileName)

                IO.File.Delete(Path + "\imem.tmp")
                IO.File.Delete(Path + "\mem.tmp")
                IO.File.Delete(Path + "\reg.tmp")
                IO.File.Delete(Path + "\port.tmp")
                IO.File.Delete(oPath + ".mem")

                Compiled = False
            Else
                MsgBox("Program is not compiled.")
            End If

        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

    Private Sub Bank_ValueChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Bank.ValueChanged

        If Bank.Value > 3 Then
            Bank.Value = 3
        End If
        ListReg(Bank.Value)

    End Sub



    Private Sub TXT_Area_TextChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TXT_Area.TextChanged
        TextChange = True
        Compiled = False
        Tip.Text = Str(TXT_Area.Lines.Length) + "  Lines   " + Str(TXT_Area.TextLength - TXT_Area.Lines.Length) + "  Chars  |  "

    End Sub

    Private Sub Main_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing

        If TextChange = True Then
            If MsgBox("Program not saved. Do you want to save it?", MsgBoxStyle.YesNo) = MsgBoxResult.Yes Then
                SaveFile()
            End If
        End If

    End Sub

    Private Sub FontToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FontToolStripMenuItem.Click
        FontDialog1.ShowDialog()
        TXT_Area.Font = FontDialog1.Font
    End Sub

    Private Sub ZoomInToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ZoomInToolStripMenuItem.Click
        TXT_Area.ZoomFactor = TXT_Area.ZoomFactor + 0.1
    End Sub

    Private Sub ZoomOutToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ZoomOutToolStripMenuItem.Click
        TXT_Area.ZoomFactor = TXT_Area.ZoomFactor - 0.1
    End Sub

    Private Sub ResetToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ResetToolStripMenuItem.Click
        TXT_Area.ZoomFactor = 1
    End Sub

    Private Sub ClearToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ClearToolStripMenuItem.Click
        MSG.Items.Clear()
    End Sub

    Private Sub Main_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        WorkDir = IO.Directory.GetCurrentDirectory
        Tip.Text = Str(TXT_Area.Lines.Length) + "  Lines   " + Str(TXT_Area.TextLength - TXT_Area.Lines.Length) + "  Chars  |  "
    End Sub

    Private Sub MToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MToolStripMenuItem.Click
        Dim tmp1(), tmp2() As String

        Try
            If TXT_Area.Text.Contains("start_mem") = False Then
                ReDim tmp2(TXT_Area.Lines.Length + 3)

                tmp1 = TXT_Area.Lines

                tmp2(0) = "start_mem {"
                tmp2(1) = ""
                tmp2(2) = "} end_mem"

                Dim cnt As Integer = 0
                While cnt < tmp1.Length
                    tmp2(cnt + 3) = tmp1(cnt)
                    cnt += 1
                End While

                TXT_Area.Lines = tmp2
                MSG.Items.Add("Memory block added.")
                MSG.SelectedIndex = MSG.Items.Count - 1
            Else
                MSG.Items.Add("Memory block exists")
                MSG.SelectedIndex = MSG.Items.Count - 1
                My.Computer.Audio.PlaySystemSound(Media.SystemSounds.Beep)
            End If

        Catch ex As Exception
            MsgBox(ex.Message)
        End Try

    End Sub

    Private Sub ORGBlockToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ORGBlockToolStripMenuItem.Click
        Dim tmp1(), tmp2() As String

        Try
            If TXT_Area.Text.Contains("org") = False Then

                ReDim tmp2(TXT_Area.Lines.Length + 3)
                Dim cnt As Integer = 0
                tmp1 = TXT_Area.Lines

                While cnt < tmp1.Length
                    tmp2(cnt) = tmp1(cnt)
                    cnt += 1
                End While

                tmp2(cnt) = "org {"
                tmp2(cnt + 1) = ""
                tmp2(cnt + 2) = "} end"

                TXT_Area.Lines = tmp2
                MSG.Items.Add("ORG block added.")
                MSG.SelectedIndex = MSG.Items.Count - 1
            Else
                MSG.Items.Add("ORG block exists.")
                MSG.SelectedIndex = MSG.Items.Count - 1
                My.Computer.Audio.PlaySystemSound(Media.SystemSounds.Beep)
            End If


        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

    Private Sub AboutToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AboutToolStripMenuItem.Click
        AboutBox1.ShowDialog()
    End Sub

    Private Sub ContentsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ContentsToolStripMenuItem.Click
        Contents.Show()
    End Sub
End Class
