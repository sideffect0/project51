

Module Functions

    Dim fn As Integer
    Dim Reg(15, 4), PSW(8), Port(8, 4) As String

    Public Sub NewFile()
        Try
            If Main.TextChange = True Then
                If MsgBox("Save changes to current file?", MsgBoxStyle.YesNo) = MsgBoxResult.Yes Then
                    If Main.CurFileName = "Untitled.asm" Then
                        Main.SaveFileDialog.Filter = "ASM Files (*.ASM)|*.asm"
                        Main.SaveFileDialog.FilterIndex = 0
                        Main.SaveFileDialog.ShowDialog()
                        Main.CurFileName = Main.SaveFileDialog.FileName
                        fn = FreeFile()
                        FileOpen(fn, Main.CurFileName, OpenMode.Output)
                        Print(fn, Main.TXT_Area.Text)
                        FileClose(fn)
                        Main.TextChange = False
                    Else
                        fn = FreeFile()
                        FileOpen(fn, Main.CurFileName, OpenMode.Output)
                        Print(fn, Main.TXT_Area.Text)
                        FileClose(fn)
                        Main.TextChange = False
                    End If
                    Main.TXT_Area.Clear()
                    Main.CurFileName = "Untitled.asm"
                    Main.Text = "sim51 Editor - " + Main.CurFileName
                    Main.TextChange = False
                    Main.MSG.Items.Clear()
                Else
                    Main.TXT_Area.Clear()
                    Main.CurFileName = "Untitled.asm"
                    Main.Text = "sim51 Editor - " + Main.CurFileName
                    Main.TextChange = False
                    Main.MSG.Items.Clear()
                End If
            Else
                Main.TXT_Area.Clear()
                Main.CurFileName = "Untitled.asm"
                Main.Text = "sim51 Editor - " + Main.CurFileName
                Main.TextChange = False
                Main.MSG.Items.Clear()
            End If
            Main.Tip.Text = Str(Main.TXT_Area.Lines.Length) + "  Lines   " + Str(Main.TXT_Area.TextLength - Main.TXT_Area.Lines.Length) + "  Chars  |  "
            ListReg(-1)
        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

    Public Sub SaveFile()

        If Main.TextChange = True Then
            If Main.CurFileName = "Untitled.asm" Then
                Main.SaveFileDialog.Filter = "ASM Files (*.ASM)|*.asm"
                Main.SaveFileDialog.FilterIndex = 0
                Main.OpenFileDialog.FileName = ""
                Main.SaveFileDialog.ShowDialog()
                If Main.SaveFileDialog.FileName <> "" Then
                    Main.CurFileName = Main.SaveFileDialog.FileName
                    fn = FreeFile()
                    FileOpen(fn, Main.CurFileName, OpenMode.Output)
                    Print(fn, Main.TXT_Area.Text)
                    FileClose(fn)
                    Main.TextChange = False
                    Main.MSG.Items.Add("File save successfully.")
                    Main.MSG.SelectedIndex = Main.MSG.Items.Count - 1
                    Main.Text = "sim51 Editor - " + Main.CurFileName
                Else
                    Main.Text = "sim51 Editor - " + Main.CurFileName
                    Main.TextChange = False
                End If
            ElseIf Main.TXT_Area.Text <> "" Then
                Try
                    fn = FreeFile()
                    FileOpen(fn, Main.CurFileName, OpenMode.Output)
                    Print(fn, Main.TXT_Area.Text)
                    FileClose(fn)
                    Main.TextChange = False
                    Main.MSG.Items.Add("File save successfully.")
                    Main.MSG.SelectedIndex = Main.MSG.Items.Count - 1
                Catch ex As Exception
                    MsgBox(ex.Message)
                End Try
                Main.TextChange = False
                Main.Text = "sim51 Editor - " + Main.CurFileName

            End If
        End If
        Main.Tip.Text = Str(Main.TXT_Area.Lines.Length) + "  Lines   " + Str(Main.TXT_Area.TextLength - Main.TXT_Area.Lines.Length) + "  Chars  |  "
    End Sub

    Public Sub OpenFile()
        Try
            Dim fh As IO.StreamReader
            Main.OpenFileDialog.Filter = "ASM Files(*.ASM)|*.asm"
            Main.OpenFileDialog.FileName = ""
            Main.OpenFileDialog.ShowDialog()
            fh = IO.File.OpenText(Main.OpenFileDialog.FileName)
            Main.TXT_Area.Text = fh.ReadToEnd()
            Main.CurFileName = Main.OpenFileDialog.FileName
            Main.Text = "sim51 Editor - " + Main.CurFileName
            Main.TextChange = False
            Main.MSG.Items.Clear()
            Main.MSG.Items.Add("Ready.")
            Main.MSG.SelectedIndex = Main.MSG.Items.Count - 1
            fh.Close()
            Main.Tip.Text = Str(Main.TXT_Area.Lines.Length) + "  Lines   " + Str(Main.TXT_Area.TextLength - Main.TXT_Area.Lines.Length) + "  Chars  |  "

        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
        ListReg(-1)
    End Sub

    Public Sub ListMem()

        Dim Path As String = IO.Path.GetDirectoryName(Main.CurFileName)
        Path = Path + "\mem.tmp"
        Dim lines() As String

        Main.Xmem.Items.Clear()
        Main.IMem.Items.Clear()

        Try
            lines = IO.File.ReadAllLines(Path)
            Dim Count As Integer = 0

            If lines(Count) = "XMEM" Then
                Count = Count + 1
                While lines(Count) <> "IMEM"
                    If lines(Count) = "" Then
                        'MsgBox(lines(Count))
                        lines(Count) = "0"
                    End If
                    Main.Xmem.Items.Add(lines(Count))
                    Count = Count + 1
                End While

                If lines(Count) = "IMEM" Then
                    Count = Count + 1
                    While lines(Count) <> "END"
                        Main.IMem.Items.Add(lines(Count))
                        Count = Count + 1
                    End While

                End If

            End If

        Catch ex As Exception

        End Try

    End Sub



    Public Sub LoadReg()

        Dim Path As String = IO.Path.GetDirectoryName(Main.CurFileName)
        Path = Path + "\reg.tmp"
        Dim lines() As String
        Try
            lines = IO.File.ReadAllLines(Path)
            Dim Count As Integer = 0
            Dim index As Integer = 0

            If lines(Count) = "BANK0" Then
                Count = Count + 1
                While lines(Count) <> "BANK1"
                    Reg(index, 0) = lines(Count)

                    If Reg(index, 0) = "" Then
                        Reg(index, 0) = "0"
                    End If

                    Count = Count + 1
                    index = index + 1
                End While

                index = 0

                If lines(Count) = "BANK1" Then
                    Count = Count + 1
                    While lines(Count) <> "BANK2"
                        Reg(index, 1) = lines(Count)

                        If Reg(index, 1) = "" Then
                            Reg(index, 1) = "0"
                        End If

                        Count = Count + 1
                        index = index + 1
                    End While

                    index = 0

                    If lines(Count) = "BANK2" Then
                        Count = Count + 1
                        While lines(Count) <> "BANK3"
                            Reg(index, 2) = lines(Count)

                            If Reg(index, 2) = "" Then
                                Reg(index, 2) = "0"
                            End If

                            Count = Count + 1
                            index = index + 1

                        End While

                        index = 0

                        If lines(Count) = "BANK3" Then
                            Count = Count + 1
                            While lines(Count) <> "PSW"
                                Reg(index, 3) = lines(Count)

                                If Reg(index, 3) = "" Then
                                    Reg(index, 3) = "0"
                                End If

                                Count = Count + 1
                                index = index + 1

                            End While

                            index = 0

                            If lines(Count) = "PSW" Then
                                Count = Count + 1
                                While lines(Count) <> "END"
                                    PSW(index) = lines(Count)

                                    If PSW(index) = "" Then
                                        PSW(index) = "0"
                                    End If

                                    Count = Count + 1
                                    index = index + 1

                                End While

                            End If
                        End If
                    End If
                End If
            End If

        Catch ex As Exception

        End Try

    End Sub

    Public Sub LoadPort()
        Dim Path As String = IO.Path.GetDirectoryName(Main.CurFileName)
        Path = Path + "\port.tmp"
        Dim lines() As String
        Try
            lines = IO.File.ReadAllLines(Path)
            Dim Count As Integer = 0
            Dim index As Integer = 0

            If lines(Count) = "PORT0" Then
                Count = Count + 1
                While lines(Count) <> "PORT1"
                    Port(index, 0) = lines(Count)

                    If Port(index, 0) = "" Then
                        Port(index, 0) = "0"
                    End If

                    Count = Count + 1
                    index = index + 1
                End While

                index = 0

                If lines(Count) = "PORT1" Then
                    Count = Count + 1
                    While lines(Count) <> "PORT2"
                        Port(index, 1) = lines(Count)

                        If Port(index, 1) = "" Then
                            Port(index, 1) = "0"
                        End If

                        Count = Count + 1
                        index = index + 1
                    End While

                    index = 0

                    If lines(Count) = "PORT2" Then
                        Count = Count + 1
                        While lines(Count) <> "PORT3"
                            Port(index, 2) = lines(Count)

                            If Port(index, 2) = "" Then
                                Port(index, 2) = "0"
                            End If

                            Count = Count + 1
                            index = index + 1

                        End While

                        index = 0

                        If lines(Count) = "PORT3" Then
                            Count = Count + 1
                            While lines(Count) <> "END_PORT_BITS"
                                Port(index, 3) = lines(Count)

                                If Port(index, 3) = "" Then
                                    Port(index, 3) = "0"
                                End If

                                Count = Count + 1
                                index = index + 1

                            End While

                            index = 0
                            Main.TabPage5.ToolTipText = lines(Count + 1)
                            Main.TabPage6.ToolTipText = lines(Count + 2)
                            Main.TabPage7.ToolTipText = lines(Count + 3)
                            Main.TabPage8.ToolTipText = lines(Count + 4)

                        End If
                    End If
                End If
            End If

        Catch ex As Exception
            MsgBox(ex.Message)
        End Try
    End Sub

    Public Sub ListReg(ByVal Bank As Integer)

        If Bank = -1 Then

            Main.IMem.Items.Clear()
            Main.Xmem.Items.Clear()
            Main.Bank.Value = 0
            With Main
                .a.Text = "0"
                .b.Text = "0"
                .r0.Text = "0"
                .r1.Text = "0"
                .r2.Text = "0"
                .r3.Text = "0"
                .r4.Text = "0"
                .r5.Text = "0"
                .r6.Text = "0"
                .r7.Text = "0"
                .dptr.Text = "0"
                .cy.Text = "0"
                .ac.Text = "0"
                .fo.Text = "0"
                .rs1.Text = "0"
                .rs0.Text = "0"
                .ov.Text = "0"
                .usr.Text = "0"
                .p.Text = "0"

                .p00.Text = "0"
                .p01.Text = "0"
                .p02.Text = "0"
                .p03.Text = "0"
                .p04.Text = "0"
                .p05.Text = "0"
                .p06.Text = "0"
                .p07.Text = "0"

                .p10.Text = "0"
                .p11.Text = "0"
                .p12.Text = "0"
                .p13.Text = "0"
                .p14.Text = "0"
                .p15.Text = "0"
                .p16.Text = "0"
                .p17.Text = "0"

                .p20.Text = "0"
                .p21.Text = "0"
                .p22.Text = "0"
                .p23.Text = "0"
                .p24.Text = "0"
                .p25.Text = "0"
                .p26.Text = "0"
                .p27.Text = "0"

                .p30.Text = "0"
                .p31.Text = "0"
                .p32.Text = "0"
                .p33.Text = "0"
                .p34.Text = "0"
                .p35.Text = "0"
                .p36.Text = "0"
                .p37.Text = "0"

            End With

        Else
            With Main
                .a.Text = Reg(0, 0)
                .b.Text = Reg(1, 0)
                .dptr.Text = Reg(2, 0)
                .r1.Text = Reg(3, Bank)
                .r2.Text = Reg(4, Bank)
                .r3.Text = Reg(5, Bank)
                .r4.Text = Reg(6, Bank)
                .r5.Text = Reg(7, Bank)
                .r6.Text = Reg(8, Bank)
                .r7.Text = Reg(9, Bank)
                .r0.Text = Reg(10, Bank)

                .cy.Text = PSW(0)
                .ac.Text = PSW(1)
                .fo.Text = PSW(2)
                .rs1.Text = PSW(4)
                .rs0.Text = PSW(3)
                .ov.Text = PSW(5)
                .usr.Text = PSW(6)
                .p.Text = PSW(7)
            End With


        End If

    End Sub

    Public Sub ListrPort()

        With Main
            .p00.Text = Port(0, 0)
            .p01.Text = Port(1, 0)
            .p02.Text = Port(2, 0)
            .p03.Text = Port(3, 0)
            .p04.Text = Port(4, 0)
            .p05.Text = Port(5, 0)
            .p06.Text = Port(6, 0)
            .p07.Text = Port(7, 0)

            .p10.Text = Port(0, 1)
            .p11.Text = Port(1, 1)
            .p12.Text = Port(2, 1)
            .p13.Text = Port(3, 1)
            .p14.Text = Port(4, 1)
            .p15.Text = Port(5, 1)
            .p16.Text = Port(6, 1)
            .p17.Text = Port(7, 1)

            .p20.Text = Port(0, 2)
            .p21.Text = Port(1, 2)
            .p22.Text = Port(2, 2)
            .p23.Text = Port(3, 2)
            .p24.Text = Port(4, 2)
            .p25.Text = Port(5, 2)
            .p26.Text = Port(6, 2)
            .p27.Text = Port(7, 2)

            .p30.Text = Port(0, 3)
            .p31.Text = Port(1, 3)
            .p32.Text = Port(2, 3)
            .p33.Text = Port(3, 3)
            .p34.Text = Port(4, 3)
            .p35.Text = Port(5, 3)
            .p36.Text = Port(6, 3)
            .p37.Text = Port(7, 3)

        End With
    End Sub

End Module
