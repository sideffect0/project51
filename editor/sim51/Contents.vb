Public Class Contents

    Private Sub Contents_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Try
            RichTextBox1.LoadFile("help\Program structure.rtf")
            RichTextBox2.LoadFile("help\instructions supported.rtf")
        Catch ex As Exception

        End Try

    End Sub
End Class