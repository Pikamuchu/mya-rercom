VERSION 5.00
Object = "{F9043C88-F6F2-101A-A3C9-08002B2F49FB}#1.2#0"; "COMDLG32.OCX"
Object = "{831FDD16-0C5C-11D2-A9FC-0000F8754DA1}#2.0#0"; "MSCOMCTL.OCX"
Begin VB.Form frmDib 
   BackColor       =   &H80000016&
   Caption         =   "GUI_Rercom"
   ClientHeight    =   3900
   ClientLeft      =   60
   ClientTop       =   345
   ClientWidth     =   6765
   LinkTopic       =   "Form1"
   ScaleHeight     =   3900
   ScaleWidth      =   6765
   Begin MSComctlLib.StatusBar StatusBar1 
      Align           =   2  'Align Bottom
      DragMode        =   1  'Automatic
      Height          =   255
      Left            =   0
      TabIndex        =   8
      Top             =   3645
      Width           =   6765
      _ExtentX        =   11933
      _ExtentY        =   450
      Style           =   1
      _Version        =   393216
      BeginProperty Panels {8E3867A5-8586-11D1-B16A-00C0F0283628} 
         NumPanels       =   1
         BeginProperty Panel1 {8E3867AB-8586-11D1-B16A-00C0F0283628} 
         EndProperty
      EndProperty
   End
   Begin VB.TextBox Text1 
      Height          =   285
      Left            =   120
      TabIndex        =   7
      Text            =   "Text1"
      Top             =   3120
      Width           =   4935
   End
   Begin VB.CommandButton btnFiltra 
      Caption         =   "Filtra"
      Height          =   255
      Left            =   3120
      TabIndex        =   6
      ToolTipText     =   "Filters the loaded file"
      Top             =   2760
      Width           =   615
   End
   Begin MSComDlg.CommonDialog CommonDialog1 
      Left            =   3960
      Top             =   2640
      _ExtentX        =   847
      _ExtentY        =   847
      _Version        =   393216
   End
   Begin VB.Timer Timer1 
      Enabled         =   0   'False
      Interval        =   100
      Left            =   4560
      Top             =   2640
   End
   Begin VB.CommandButton btnStop 
      Caption         =   "Stop"
      Height          =   255
      Left            =   2280
      TabIndex        =   4
      ToolTipText     =   "Stops"
      Top             =   2760
      Width           =   615
   End
   Begin VB.CommandButton btnPause 
      Caption         =   "Pause"
      Height          =   255
      Left            =   1560
      TabIndex        =   3
      ToolTipText     =   "Pauses"
      Top             =   2760
      Width           =   615
   End
   Begin VB.CommandButton btnPlay 
      Caption         =   "Play"
      Height          =   255
      Left            =   840
      TabIndex        =   2
      ToolTipText     =   "Plays a loaded file"
      Top             =   2760
      Width           =   615
   End
   Begin VB.CommandButton btnLoad 
      Caption         =   "Load"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      ToolTipText     =   "Loads a wav File"
      Top             =   2760
      Width           =   615
   End
   Begin VB.PictureBox pctBox 
      AutoRedraw      =   -1  'True
      BackColor       =   &H00FFFFFF&
      ForeColor       =   &H000000FF&
      Height          =   2295
      Left            =   240
      ScaleHeight     =   149
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   317
      TabIndex        =   0
      ToolTipText     =   "Selección con boton izquierdo para zoom in. Click con botón derecho para zoom out full."
      Top             =   240
      Width           =   4815
   End
   Begin VB.Label Label2 
      Height          =   255
      Left            =   3120
      TabIndex        =   5
      Top             =   2760
      Width           =   615
   End
End
Attribute VB_Name = "frmDib"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Declare Function ShellExecute Lib "shell32.dll" Alias "ShellExecuteA" (ByVal hwnd As Long, ByVal lpOperation As String, ByVal lpFile As String, ByVal lpParameters As String, ByVal lpDirectory As String, ByVal nShowCmd As Long) As Long

Private Declare Function mciSendCommand Lib "winmm.dll" Alias "mciSendCommandA" (ByVal wDeviceID As Long, ByVal uMessage As Long, ByVal dwParam1 As Long, ByRef dwParam2 As Any) As Long
Private Declare Function mciGetErrorString Lib "winmm.dll" Alias "mciGetErrorStringA" (ByVal dwError As Long, ByVal lpstrBuffer As String, ByVal uLength As Long) As Long

Private Declare Function GetFileSize Lib "kernel32" (ByVal hFile As Long, lpFileSizeHigh As Long) As Long
Private Declare Function CloseHandle Lib "kernel32" (ByVal hObject As Long) As Long

Private Declare Function CreateFile Lib "kernel32" Alias "CreateFileA" (ByVal lpFileName As String, ByVal dwDesiredAccess As Long, ByVal dwShareMode As Long, lpSecurityAttributes As Long, ByVal dwCreationDisposition As Long, ByVal dwFlagsAndAttributes As Long, ByVal hTemplateFile As Long) As Long
Private Declare Function ReadFile Lib "kernel32" (ByVal hFile As Long, lpBuffer As Any, ByVal nNumberOfBytesToRead As Long, lpNumberOfBytesRead As Long, lpOverlapped As Long) As Long

Private Declare Function Rectangle Lib "gdi32" (ByVal hdc As Long, ByVal X1 As Long, ByVal Y1 As Long, ByVal X2 As Long, ByVal Y2 As Long) As Long

'Private Declare Function GlobalAlloc Lib "kernel32" (ByVal wFlags As Long, ByVal dwBytes As Long) As Long
'Private Declare Function GlobalFree Lib "kernel32" (ByVal hMem As Long) As Long

Private Declare Function ShellExecuteEx Lib "shell32.dll" Alias "ShellExecuteExA" (lpExecInfo As SHELLEXECUTEINFO) As Boolean
Private Declare Function WaitForSingleObject Lib "kernel32.dll" (ByVal hHandle As Long, ByVal dwMilliseconds As Long) As Long

Private Const MCI_OPEN = &H803
Private Const MCI_OPEN_ALIAS = &H400&
Private Const MCI_OPEN_ELEMENT = &H200&
Private Const MCI_OPEN_TYPE = &H2000&
Private Const MCI_SET = &H80D
Private Const MCI_FORMAT_MILLISECONDS = 0
Private Const MCI_SET_TIME_FORMAT = &H400&

Private Const MCI_STATUS_LENGTH = &H1&
Private Const MCI_STATUS_ITEM = &H100&
Private Const MCI_STATUS = &H814
Private Const MCI_STATUS_POSITION = &H2&

Private Const MCI_FROM = &H4&
Private Const MCI_TO = &H8&
Private Const MCI_STOP = &H808
Private Const MCI_PAUSE = &H809
Private Const MCI_RESUME = &H855
Private Const MCI_PLAY = &H806
Private Const MCI_CLOSE = &H804

Private Const WAIT_TIMEOUT = &H102

Private Const GMEM_FIXED = &H0
Private Const GMEM_ZEROINIT = &H40

Private Const INVALID_HANDLE_VALUE = -1

Private Const GENERIC_READ = &H80000000
Private Const FILE_SHARE_READ = &H1
Private Const OPEN_EXISTING = 3

Private Const SEE_MASK_FLAG_DDEWAIT = &H100
Private Const SEE_MASK_NOCLOSEPROCESS = &H40

Private Const SW_HIDE = 0
Private Const SW_SHOWNORMAL = 1

Private Const WAV_LIMIT = 60000 '1 segundo

Private Type MCI_PLAY_PARMS
    dwCallback As Long
    dwFrom As Long
    dwTo As Long
End Type

Private Type MCI_OPEN_PARMS
    dwCallback As Long
    wDeviceID As Long
    lpstrDeviceType As String
    lpstrElementName As String
    lpstrAlias As String
End Type

Private Type MCI_STATUS_PARMS
    dwCallback As Long
    dwReturn As Long
    dwItem As Long
    dwTrack As Integer
End Type

Private Type MCI_SET_PARMS
    dwCallback As Long
    dwTimeFormat As Long
    dwAudio As Long
End Type

Private Type SHELLEXECUTEINFO
  cbSize As Long
  fMask As Long
  hwnd As Long
  lpVerb As String
  lpFile As String
  lpParameters As String
  lpDirectory As String
  nShow As Long
  hInstApp As Long
  lpIDList As Long
  lpClass As String
  hkeyClass As Long
  dwHotKey As Long
  hIcon As Long
  hProcess As Long
End Type

Dim sendstring As String
Dim resultado As Long
Dim error As String
Dim cadena As String
Dim slider As Boolean
Dim mci_started As Boolean

'ficheros temporales
Dim tmpFile As String

Dim masLoad As Boolean
Dim masFiltra As Boolean

'Vectores que almacenaran los archivos de audio
Dim wavIn() As Integer
Dim wavOut() As Integer
Dim wavSize As Long

'Duracion del wav
Dim wavLength As Long

'Parametros que controlan el zoom de la imagen de señal de voz
Dim zoomX1 As Double
Dim zoomX2 As Double
Dim zoomY1 As Double
Dim zoomY2 As Double

'Coordenadas para calcular el Zoom
Dim pctBoxMouseX1 As Single
Dim pctBoxMouseY1 As Single

'Coordenadas para el rectangulo de seleccion
Dim pctBoxMouseMoveXAnt As Long
Dim pctBoxMouseMoveYAnt As Long
'flag de boton apretado en el pictureBox
Dim pctBoxMouseDowned As Boolean

'coordenada anterior de linia de progreso
Dim statusXant As Long

Dim mciopen As MCI_OPEN_PARMS
Dim mcistatus As MCI_STATUS_PARMS
Dim mciplay As MCI_PLAY_PARMS
Dim mciset As MCI_SET_PARMS

Private Sub btnFiltra_Click()
           
    Dim Retval As Long
    Dim ShExInfo As SHELLEXECUTEINFO
           
    With ShExInfo
        .cbSize = Len(ShExInfo)
        .fMask = SEE_MASK_NOCLOSEPROCESS Or SEE_MASK_FLAG_DDEWAIT
        .hwnd = Me.hwnd
        .lpVerb = "Open"
        .lpFile = "Rercom.exe"
        .lpParameters = """" & Text1.Text & """" & " """ & tmpFile & """"
        .lpDirectory = App.Path
        .nShow = SW_HIDE
    End With
       
    If Text1.Text <> "" Then
    
        'Se deshabilita el boton de filtrar
        btnFiltra.Enabled = False
        
        If masFiltra Then
            Kill tmpFile
        End If
    
        If ShellExecuteEx(ShExInfo) Then

            Do
                DoEvents
            Loop Until WaitForSingleObject(ShExInfo.hProcess, 100) <> WAIT_TIMEOUT
                        
            'MsgBox "El wait devuelve " & WaitForSingleObject(ShExInfo.hProcess, INFINITE) & " El handle es " & ShExInfo.hProcess
                     
            CloseHandle ShExInfo.hProcess
                        
            If loadWav(tmpFile) Then
    
                loadOutput (tmpFile)
                dibuja
                
                masFiltra = True
                                
            Else
                
                MsgBox "Se ha producido un error al cargar el wav filtrado"
                btnFiltra.Enabled = True
    
            End If
        
        Else
        
            MsgBox "Se ha producido un error"
            btnFiltra.Enabled = True
        
        End If

    End If
        
End Sub

Private Sub Form_Load()
    
    init
    Form_Resize

End Sub

Private Sub init()
              
    Text1.Text = ""
    
    Timer1.Interval = 10 '10 milisegundos
                
    mciopen.wDeviceID = 0
    mciopen.lpstrAlias = "wavAlias" & App.ThreadID
    mciopen.lpstrDeviceType = "waveaudio"
        
    mcistatus.dwReturn = 0
    wavLength = 0
    
    btnPlay.Enabled = False
    btnPause.Enabled = False
    btnStop.Enabled = False
    btnFiltra.Enabled = False
        
    tmpFile = App.Path & "\wff" & App.ThreadID & ".tmp"
    
    masLoad = False
    masFiltra = False
    
    zoomX1 = 0
    zoomX2 = 1
    zoomY1 = 0
    zoomY2 = 1
        
End Sub

Private Sub Form_Resize()
    On Error Resume Next
    
    Text1.Move 120, 120, Me.ScaleWidth - 240, 240
    pctBox.Move 120, 420, Me.ScaleWidth - 240, Me.ScaleHeight - 1200
    btnLoad.Move 120, Me.ScaleHeight - 720, 840, 360
    btnPlay.Move 1020, Me.ScaleHeight - 720, 840, 360
    btnPause.Move 1920, Me.ScaleHeight - 720, 840, 360
    btnStop.Move 2820, Me.ScaleHeight - 720, 840, 360
    btnFiltra.Move 3720, Me.ScaleHeight - 720, 840, 360
    StatusBar1.Height = 320
    Label2.Move Me.ScaleWidth - 1080, Me.ScaleHeight - 720, 840, 360
    
    dibuja
                
End Sub

Public Sub loadInput(file As String)

    Dim inputWav As Long
    Dim tmp As Integer
    Dim ret As Long
    Dim res As Long
    Dim i As Long
    Dim cabecera(24) As Integer
                            
    ' Abrimos el fichero wav original
    inputWav = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, ByVal 0&, OPEN_EXISTING, 0, 0)
    
    If inputWav = INVALID_HANDLE_VALUE Then
        MsgBox "error al cargar el archivo"
        Exit Sub
    End If
    
    wavSize = GetFileSize(inputWav, 0)
    wavSize = (wavSize - 24) / Len(tmp)
    
    'MsgBox "wavSize es " & wavSize
    
    ReDim wavIn(wavSize)
    ReDim wavOut(wavSize)
    
    For i = 0 To 23
        res = ReadFile(inputWav, tmp, Len(tmp), ret, ByVal 0&)
        cabecera(i) = tmp
    Next
    
    For i = 0 To (wavSize - 1)
        res = ReadFile(inputWav, tmp, Len(tmp), ret, ByVal 0&)
        wavIn(i) = tmp
    Next
    
    CloseHandle inputWav
    
End Sub

Public Sub loadOutput(file As String)

    Dim outputWav As Long
    Dim tmp As Integer
    Dim ret As Long
    Dim res As Long
    Dim i As Long
    Dim cabecera(24) As Integer
                        
    ' Abrimos el fichero wav original
    outputWav = CreateFile(file, GENERIC_READ, FILE_SHARE_READ, ByVal 0&, OPEN_EXISTING, 0, 0)
    
    If inputWav = INVALID_HANDLE_VALUE Then
        MsgBox "error al cargar el archivo"
        Exit Sub
    End If
          
    For i = 0 To 23
        res = ReadFile(outputWav, tmp, Len(tmp), ret, ByVal 0&)
        cabecera(i) = tmp
    Next
    
    For i = 0 To (wavSize - 1)
        res = ReadFile(outputWav, tmp, Len(tmp), ret, ByVal 0&)
        wavOut(i) = tmp
    Next
    
    CloseHandle outputWav
    
End Sub

Public Sub dibuja()
    If Not masLoad Then Exit Sub
      
    Dim X1 As Long
    Dim X2 As Long
    Dim Y1 As Long
    Dim Y2 As Long
    Dim wavMax As Long
    Dim muestras As Long
    Dim i As Long
    Dim Xinicio As Long
    Dim Xzoom As Long
    Dim Ycentral As Long
    Dim factorAmpli As Double
    Dim factorTmp As Double
                                
    i = 0
    nBytes = 0
    muestras = 0
    cabecera = 0
    wavMax = (2 ^ (8 * 2)) 'Para señales de audio de 16 bits
    X2 = 0
    X1 = 0
    pctBox.Cls
    
    'Ajustamos imagen a los valores de zoom
    Xinicio = wavSize * zoomX1
    Xzoom = wavSize * (zoomX2 - zoomX1)
    'mediaYzoom = (zoomY1 + zoomY2) / 2
        
    factorAmpli = pctBox.ScaleHeight / (wavMax * (zoomY2 - zoomY1))
    'Ycentral = pctBox.ScaleHeight / 2
           
    'factorTmp = (0.5 / zoomY2) - (2 * zoomY1 * zoomY1)
    factorTmp = (0.5 / zoomY2) - (zoomY1)
    Ycentral = pctBox.ScaleHeight * factorTmp
    
    Y2 = Ycentral
    Y1 = Ycentral
    
    'MsgBox "factorAmpli norm es " & factorAmpli / pctBox.ScaleHeight & " Ycentral norm es " & Ycentral / pctBox.ScaleHeight & " zoomY1 es " & zoomY1 & " zoomY2 es " & zoomY2
    
    If Xzoom <= 0 Then
        MsgBox "Xzoom es " & Xzoom
        Exit Sub
    End If
            
    pctBox.ForeColor = &HFF&
    pctBox.DrawStyle = 0
    pctBox.DrawMode = 13
    
    escribirMensaje "Dibujando."
        
    For i = 0 To Xzoom
                                
        'Dibujamos en pctBox
        X1 = (i * pctBox.ScaleWidth) / Xzoom
        Y1 = (wavIn(Xinicio + i) * factorAmpli) + Ycentral
            
        'MsgBox "insertando punto en (" & X1 & ", " & Y1 & ")"
        'pctBox.PSet (X, Y)
        pctBox.Line (X1, Y1)-(X2, Y2)
            
        X2 = X1
        Y2 = Y1
                    
    Next
    
    X2 = 0
    Y2 = pctBox.ScaleHeight / 2
    
    pctBox.ForeColor = &HFF00&
    pctBox.DrawStyle = 0
    pctBox.DrawMode = 13
    
    escribirMensaje "Dibujando.."
    
    For i = 0 To Xzoom
                                
        'Dibujamos en pctBox
        X1 = (i * pctBox.ScaleWidth) / Xzoom
        'Y1 = ((wavOut(Xinicio + i) * pctBox.ScaleHeight) / wavMax) + (pctBox.ScaleHeight / 2)
        Y1 = (wavOut(Xinicio + i) * factorAmpli) + Ycentral
            
        'MsgBox "insertando punto en (" & X1 & ", " & Y1 & ")"
        'pctBox.PSet (X, Y)
        pctBox.Line (X1, Y1)-(X2, Y2)
            
        X2 = X1
        Y2 = Y1
                    
    Next
    
    escribirMensaje "Dibujado"
    
    'MsgBox "fin"
   
End Sub

Private Sub btnLoad_Click()
    With CommonDialog1
        .Filter = "Archivos *.wav | *.wav"
        .ShowOpen
        Text1.Text = .FileName
    End With
    
    Dim res As Boolean
                   
    If loadWav(Text1.Text) Then
    
        loadInput (Text1.Text)
                
        masLoad = True
        dibuja
                
        'Habilitamos el botón de filtrar
        btnFiltra.Enabled = True
            
    End If
    
End Sub

Private Sub btnPlay_Click()
    
    Timer1.Enabled = True
    mciplay.dwFrom = wavLength * zoomX1
    mciplay.dwTo = wavLength * zoomX2
    resultado = mciSendCommand(mciopen.wDeviceID, MCI_PLAY, MCI_FROM Or MCI_TO, mciplay)
    BuscarError resultado
    liniaProgreso mcistatus.dwReturn, mciplay.dwFrom, mciplay.dwTo, True
    
End Sub

Sub BuscarError(número As Long)
    error = Space(255)
    mciGetErrorString resultado, error, 255
    escribirMensaje error
End Sub


Private Sub btnStop_Click()
    
    resultado = mciSendCommand(mciopen.wDeviceID, MCI_STOP, 0, 0)
    BuscarError resultado
    Timer1.Enabled = False
        
End Sub

Private Sub btnPause_Click()
    
    If btnPause.Caption = "Pause" Then
        resultado = mciSendCommand(mciopen.wDeviceID, MCI_PAUSE, 0, 0)
        BuscarError resultado
        btnPause.Caption = "Continue"
    Else
        resultado = mciSendCommand(mciopen.wDeviceID, MCI_RESUME, 0, 0)
        BuscarError resultado
        btnPause.Caption = "Pause"
    End If
    
End Sub

Private Sub Form_Unload(Cancel As Integer)

    If masLoad Then
        resultado = mciSendCommand(mciopen.wDeviceID, MCI_CLOSE, 0, ByVal 0&)
        BuscarError resultado
    End If
    
    If masFiltra Then
        Kill tmpFile
    End If

End Sub

Private Sub pctBox_MouseDown(Button As Integer, Shift As Integer, X As Single, Y As Single)

    'MsgBox "Button es " & Button & " Shift es " & Shift & " X es " & X & " Y es " & Y
    
    If Button = 1 Then
                           
        pctBoxMouseX1 = X
        pctBoxMouseY1 = Y
        
        pctBox.ForeColor = &H0& 'black
        pctBox.DrawStyle = 2    'Dot
        pctBox.DrawMode = 10    'Not Xor Pen
                                
        pctBoxMouseDowned = True
        Rectangle pctBox.hdc, pctBoxMouseX1, pctBoxMouseY1, X, Y
        pctBoxMouseMoveXAnt = X
        pctBoxMouseMoveYAnt = Y
        pctBox.Refresh
        
    End If
     
End Sub

Private Sub pctBox_MouseMove(Button As Integer, Shift As Integer, X As Single, Y As Single)
    If Not pctBoxMouseDowned Then Exit Sub
        
    'Borramos rectangulo anterior
    Rectangle pctBox.hdc, pctBoxMouseX1, pctBoxMouseY1, pctBoxMouseMoveXAnt, pctBoxMouseMoveYAnt
        
    'Creamos nuevo rectangulo
    Rectangle pctBox.hdc, pctBoxMouseX1, pctBoxMouseY1, X, Y
    pctBoxMouseMoveXAnt = X
    pctBoxMouseMoveYAnt = Y
    pctBox.Refresh
        
End Sub

Private Sub pctBox_MouseUp(Button As Integer, Shift As Integer, X As Single, Y As Single)

    Dim zoomXdif As Double
    Dim zoomYdif As Double
    Dim tmp As Double
                      
    If Button = 1 Then
    
        'Borramos rectangulo anterior
        Rectangle pctBox.hdc, pctBoxMouseX1, pctBoxMouseY1, X, Y
        pctBox.Refresh
                        
        pctBoxMouseDowned = False
        
        zoomXdif = zoomX2 - zoomX1
        zoomYdif = zoomY2 - zoomY1
                  
        'Calculo zoom para eje X
        If pctBoxMouseX1 < X Then
            
            tmp = (X / pctBox.ScaleWidth) * zoomXdif
            zoomX2 = zoomX1 + tmp
            tmp = (pctBoxMouseX1 / pctBox.ScaleWidth) * zoomXdif
            zoomX1 = zoomX1 + tmp
            zoomY2 = 1
            zoomY1 = 0
            
        Else
        
            If pctBoxMouseX1 > X Then
            
                tmp = (pctBoxMouseX1 / pctBox.ScaleWidth) * zoomXdif
                zoomX2 = zoomX1 + tmp
                tmp = (X / pctBox.ScaleWidth) * zoomXdif
                zoomX1 = zoomX1 + tmp
                zoomY1 = 0
                zoomY2 = 1
                
            End If
        
        End If
        
        'Calculo zoom para eje Y
        If pctBoxMouseY1 < Y Then
            
            tmp = (Y / pctBox.ScaleHeight) * zoomYdif
            zoomY2 = zoomY1 + tmp
            tmp = (pctBoxMouseY1 / pctBox.ScaleHeight) * zoomYdif
            zoomY1 = zoomY1 + tmp
            
        Else
        
            If pctBoxMouseY1 > X Then
            
                tmp = (pctBoxMouseY1 / pctBox.ScaleHeight) * zoomYdif
                zoomY2 = zoomY1 + tmp
                tmp = (Y / pctBox.ScaleHeight) * zoomYdif
                zoomY1 = zoomY1 + tmp
                
            End If
        
        End If
        
        'Limitamos valor de zoom validos entre 0 y 1
        If zoomX1 < 0 Then
            zoomX1 = 0
        End If
        If zoomX2 > 1 Then
            zoomX2 = 1
        End If
        If zoomY1 < 0 Then
            zoomY1 = 0
        End If
        If zoomY2 > 1 Then
            zoomY2 = 1
        End If

    
    Else
        'Restablecemos valor de zoom
        If Button = 2 Then
            zoomX1 = 0
            zoomX2 = 1
            zoomY1 = 0
            zoomY2 = 1
        End If
    
    End If
    
    'MsgBox "zoomX1 es " & zoomX1 & " zoomX2 es " & zoomX2 & "zoomY1 es " & zoomY1 & " zoomY2 es " & zoomY2
        
    dibuja

End Sub

Private Sub Timer1_Timer()

    mcistatus.dwItem = MCI_STATUS_POSITION
    resultado = mciSendCommand(mciopen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, mcistatus)
    BuscarError resultado
    
    Label2.Caption = formatotiempo(mcistatus.dwReturn)
    liniaProgreso mcistatus.dwReturn, mciplay.dwFrom, mciplay.dwTo, False
    
    If mcistatus.dwReturn >= mciplay.dwTo Then
        Timer1.Enabled = False
    End If
                
End Sub

Function formatotiempo(milisegundos As Long) As String

    Dim segundos As Integer
    Dim minutos As Integer
        
    segundos = milisegundos \ 1000
    minutos = segundos \ 60
    segundos = segundos Mod 60
    
    If milisegundos Mod 1000 < 100 Then
        formatotiempo = minutos & " : " & segundos & " : 0" & milisegundos Mod 1000
    Else
        formatotiempo = minutos & " : " & segundos & " : " & milisegundos Mod 1000
    End If
End Function

Function loadWav(file As String) As Boolean

    If file <> "" Then
    
        mciopen.lpstrElementName = file
        
        If masLoad Then
            resultado = mciSendCommand(mciopen.wDeviceID, MCI_CLOSE, 0, ByVal 0&)
            If resultado <> 0 Then
                BuscarError resultado
                loadWav = False
                Exit Function
            End If
        End If
                
        resultado = mciSendCommand(0, MCI_OPEN, MCI_OPEN_ALIAS Or MCI_OPEN_TYPE Or MCI_OPEN_ELEMENT, mciopen)
        BuscarError resultado
        If resultado <> 0 Then
            loadWav = False
            Exit Function
        End If
            
        mciset.dwTimeFormat = MCI_FORMAT_MILLISECONDS
        resultado = mciSendCommand(mciopen.wDeviceID, MCI_SET, MCI_SET_TIME_FORMAT, mciset)
        BuscarError resultado
        If resultado <> 0 Then
            loadWav = False
            Exit Function
        End If
        
        mcistatus.dwItem = MCI_STATUS_LENGTH
        resultado = mciSendCommand(mciopen.wDeviceID, MCI_STATUS, MCI_STATUS_ITEM, mcistatus)
        BuscarError resultado
        If resultado <> 0 Then
            loadWav = False
            Exit Function
        Else
            If mcistatus.dwReturn > WAV_LIMIT Then
                escribirMensaje "Versión DEMO: solo se permite el filtrado de ficheros de audio de menos de " & WAV_LIMIT / 1000 & " segundos."
                MsgBox "Version DEMO solo se permite el filtrado de ficheros de audio de menos de " & WAV_LIMIT / 1000 & " segundos."
                masLoad = True
                loadWav = False
                Exit Function
            End If
        End If
            
        btnPlay.Enabled = True
        btnPause.Enabled = True
        btnStop.Enabled = True
        
        wavLength = mcistatus.dwReturn
    
        'ProgressBar1.Min = 1
        'ProgressBar1.Max = wavLength
        'ProgressBar1.Value = 1
        
        'Slider1.Min = 1
        'Slider1.Max = wavLength
        'Slider1.Value = 1
        
        Label2.Caption = formatotiempo(mcistatus.dwReturn)

        masLoad = True
        loadWav = True
                        
    Else
    
        loadWav = False
                
    End If

End Function

Public Sub liniaProgreso(status As Long, inicio As Long, Fin As Long, init As Boolean)

    Dim statusX As Long
        
    pctBox.ForeColor = &HFF0000  'black
    pctBox.DrawStyle = 0         'solid
    pctBox.DrawMode = 10         'Not Xor Pen
        
    If Not inicio >= Fin Then
        statusX = ((status - inicio) / (Fin - inicio)) * pctBox.ScaleWidth
    Else
        statusX = 0
    End If
                
    If Not init Then
        'Borramos linia anterior
        pctBox.Line (statusXant, 0)-(statusXant, pctBox.ScaleHeight)
    End If
                                
    pctBox.Line (statusX, 0)-(statusX, pctBox.ScaleHeight)
    pctBox.Refresh
      
    statusXant = statusX
   
End Sub

Public Sub escribirMensaje(msg As String)

    StatusBar1.SimpleText = msg
    
End Sub

