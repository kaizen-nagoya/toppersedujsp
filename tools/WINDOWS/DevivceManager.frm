VERSION 5.00
Object = "{194B359F-594E-4EE4-A804-84723D1CF1C4}#1.0#0"; "device.dll"
Begin VB.Form Form1 
   Caption         =   "OAKS16 MINI Simulater"
   ClientHeight    =   2895
   ClientLeft      =   225
   ClientTop       =   7905
   ClientWidth     =   5790
   Icon            =   "DevivceManager.frx":0000
   LinkTopic       =   "Form1"
   ScaleHeight     =   2895
   ScaleWidth      =   5790
   Begin VB.Frame Frame3 
      Caption         =   "PORT"
      Height          =   2175
      Left            =   240
      TabIndex        =   9
      Top             =   360
      Width           =   2175
      Begin VB.Label Label12 
         Caption         =   "SFR PUR2 ="
         Height          =   255
         Left            =   120
         TabIndex        =   19
         Top             =   1800
         Width           =   1095
      End
      Begin VB.Label Label9 
         Caption         =   "not init"
         Height          =   255
         Left            =   1320
         TabIndex        =   18
         Top             =   1800
         Width           =   615
      End
      Begin VB.Label Label8 
         Caption         =   "SFR PD8   ="
         Height          =   255
         Left            =   120
         TabIndex        =   17
         Top             =   1440
         Width           =   1095
      End
      Begin VB.Label Label4 
         Caption         =   "not init"
         Height          =   255
         Left            =   1320
         TabIndex        =   16
         Top             =   1440
         Width           =   735
      End
      Begin VB.Label Label7 
         Caption         =   "SFR P7     ="
         Height          =   255
         Left            =   120
         TabIndex        =   15
         Top             =   1080
         Width           =   1095
      End
      Begin VB.Label Label3 
         Caption         =   "not init"
         Height          =   255
         Left            =   1320
         TabIndex        =   14
         Top             =   1080
         Width           =   735
      End
      Begin VB.Label Label6 
         Caption         =   "SFR PD7   ="
         Height          =   375
         Left            =   120
         TabIndex        =   13
         Top             =   720
         Width           =   1095
      End
      Begin VB.Label Label2 
         Caption         =   "not init"
         Height          =   255
         Left            =   1320
         TabIndex        =   12
         Top             =   720
         Width           =   735
      End
      Begin VB.Label Label5 
         Caption         =   "SFR P7     ="
         Height          =   255
         Left            =   120
         TabIndex        =   11
         Top             =   360
         Width           =   1095
      End
      Begin VB.Label Label1 
         Caption         =   "not init"
         Height          =   255
         Left            =   1320
         TabIndex        =   10
         Top             =   360
         Width           =   735
      End
   End
   Begin VB.Frame Frame2 
      Caption         =   "SW5"
      Height          =   975
      Left            =   4080
      TabIndex        =   6
      Top             =   360
      Width           =   975
      Begin VB.OptionButton Option2 
         Caption         =   "OFF"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   8
         Top             =   600
         Value           =   -1  'True
         Width           =   735
      End
      Begin VB.OptionButton Option2 
         Caption         =   "ON"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   7
         Top             =   240
         Width           =   735
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "SW4"
      Height          =   975
      Left            =   3000
      TabIndex        =   3
      Top             =   360
      Width           =   975
      Begin VB.OptionButton Option1 
         Caption         =   "OFF"
         Height          =   255
         Index           =   1
         Left            =   120
         TabIndex        =   5
         Top             =   600
         Value           =   -1  'True
         Width           =   735
      End
      Begin VB.OptionButton Option1 
         Caption         =   "ON"
         Height          =   255
         Index           =   0
         Left            =   120
         TabIndex        =   4
         Top             =   240
         Width           =   615
      End
   End
   Begin ATLDEVICELibCtl.DeviceControl DeviceControl1 
      Height          =   375
      Left            =   5280
      OleObjectBlob   =   "DevivceManager.frx":014A
      TabIndex        =   0
      Top             =   0
      Width           =   495
   End
   Begin VB.Image Image2 
      Height          =   435
      Left            =   4200
      Picture         =   "DevivceManager.frx":0172
      Top             =   1680
      Width           =   450
   End
   Begin VB.Image Image1 
      Height          =   435
      Left            =   3120
      Picture         =   "DevivceManager.frx":0C20
      Top             =   1680
      Width           =   450
   End
   Begin VB.Label Label11 
      Caption         =   "LED3"
      Height          =   255
      Left            =   4200
      TabIndex        =   2
      Top             =   1440
      Width           =   615
   End
   Begin VB.Label Label10 
      Caption         =   "LED2"
      Height          =   255
      Left            =   3120
      TabIndex        =   1
      Top             =   1440
      Width           =   495
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
DefInt I
'ポートの定義
Dim Port8 As Integer

Private Sub Form_Load()
    DeviceControl1.Connect          'デバイスマネージャと接続する
    DeviceControl1.Map &H3ED, 1     '0x03ed番地に1バイトのレジスタをマップする
    DeviceControl1.Map &H3EF, 1     '0x03ef番地に1バイトのレジスタをマップする
    DeviceControl1.Map &H3F0, 1     '0x03f0番地に1バイトのレジスタをマップする
    DeviceControl1.Map &H3F2, 1     '0x03f2番地に1バイトのレジスタをマップする
    DeviceControl1.Map &H3FE, 1     '0x03fe番地に1バイトのレジスタをマップする
    Port8 = &HFF
    Label3 = Hex(Port8)
    SW4 = 1
    SW5 = 1
End Sub

Private Sub Form_Unload(Cancel As Integer)
    DeviceControl1.Close            'デバイスマネージャとの接続を解除する
End Sub

Private Sub DeviceControl1_OnWrite(ByVal address As Long, ByVal sz As Long)
    If address = &H3ED Then
        Label1 = Hex(DeviceControl1.Value)
        I = DeviceControl1.Value
        If ((I And &H20) / 32) = 0 Then
            Image1.Picture = LoadPicture("LED_ON.bmp")
        Else
            Image1.Picture = LoadPicture("LED_OFF.bmp")
        End If
        If ((I And &H10) / 16) = 0 Then
            Image2.Picture = LoadPicture("LED_ON.bmp")
        Else
            Image2.Picture = LoadPicture("LED_OFF.bmp")
        End If
    ElseIf address = &H3EF Then
        Label2 = Hex(DeviceControl1.Value)
    ElseIf address = &H3F2 Then
        Label4 = Hex(DeviceControl1.Value)
    Else
        Label9 = Hex(DeviceControl1.Value)
    End If
End Sub

Private Sub DeviceControl1_OnRead(ByVal address As Long, ByVal sz As Long)
    If address = &H3F0 Then
        DeviceControl1.Value = Port8
    Else
        DeviceControl1.Value = 0
    End If
End Sub

Private Sub Option1_Click(Index As Integer)
    Port8 = (Index * 2) + (Port8 And &HFD)
    Label3 = Hex(Port8)
End Sub

Private Sub Option2_Click(Index As Integer)
    Port8 = Index + (Port8 And &HFE)
    Label3 = Hex(Port8)
End Sub
