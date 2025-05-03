1.この教材のビルドに関して

開発環境の構築の仕方について説明します。
まず、2004/10/12版のWindows用のJSP1.4.1をダウンロードしてください。
解凍を行い。jspのデレクトリィをjsp-1.4.1に書き換えてください。
書き換えたデレクトリィに、このファイルを上書きしてください。
上書きの内容は、以下の３つです。
1)初級教材用コンテンツ（OBJの下のファイルです）
2)初級教材用モニター(monitor以下のファイルです)
3)カップラーメンタイマーシミュレータ（toolsの下に現状ファイルとの差分があります）

以下にm16c.txt内のsample1の構築方法を載せます。
それに、従って、OBJ以下の教育コンテンツを構築してください。
sample1と同じで、最後に小文字のmが付いたTMファイルがOAKS16-MINI用です。
付いていないものが、OAKS16用です。

OAKS16フルキットまたはOAKS16 MINIフルキットの添付されているCD-ROM中の開発環
境を用いてm16c用のTOPPERS/JSPを構築する．構築したMOTファイルをFlashStaコマ
ンド用いて、ボード上のフラッシュROMに書き込むことによりボード上で実行できる．
対応はOAKS16とOAKS16 MINIです.構築方法は以下の通りです。構築にあたっては、
windows用のコンフィギュコマンドを用いますのでconfigデレクトリィ上のcfg.exe
を構築してください。フルキットに入っているCD-ROMから統合開発環境TM,コンパイ
ラNC30WA,フラッシュROM書き込みツールFlashStaをインストールしておいて下さい．

1.1 ツールの構築

utils/m16c-renesas上のツールm16coffset.exeとm16cvec.exeを構築します.
まずm16coffsetを生成する．utils/m16c-renesas/内のMicrosoft Visual C++プロジ
ェクトファイルm16cutils.dswを開きます，リビルドメニューからリビルドを選択す
るとm16coffset.exeが作成されている．
続けて、m16cvec.exeを生成します．メニューから「プロジェクト」「アクティブプ
ロジェクトの設定」「o16cvec」を選択し、ビルドメニューからビルドを選択する。
実行ファイルはutils/m16c-renesas/m16cvec.exeとして作成されている.

1.2 TMにプロジェクトを選択しビルドする．

Jsp14sample1.tmi、Jsp14sample1.tmk、Jsp14sample1m.tmi、Jsp14sample1m.tmk中の
設定には絶対パスの指定を行われています．TMを使ってプロジェクトの設定を行う前
に、現在のtools/M16C-RENESASデレクトリィの絶対パスと異なる場合は、エデッタを
使って絶対パスの設定を書き換えてください．以下にJsp14sample1.tmiとJsp14sampl
e1.tmkの例を示します．
---Jsp14sample1.tmi ---
[PROJECT]
ProjectName=Jsp14sample1
WorkingDirectory=D:\usr\TOPPERS\jsp-1.4.1\tools\R16C-RENESAS			<-絶対パス
OutputDirectory=.
ProjectFile=D:\usr\TOPPERS\jsp-1.4.1\tools\R16C-RENESAS\Jsp14sample1.tmk<-絶対パス
ProjectInfoFile=D:\usr\TOPPERS\jsp-1.4.1\tools\R16C-RENESAS\Jsp14sample1.tmi<-絶対パス
TargetName=M16C Family
Type=1
GoalFile=D:\usr\TOPPERS\jsp-1.4.1\tools\R16C-RENESAS\Jsp14sample1.x30<-絶対パス
[NC]
Name=NC30WA
Version=V.5.10 Release 1
	:

---Jsp14sample1.tmk ---
####################################################################
# Makefile for TM V.3.20
# Copyright 1998-2002 MITSUBISHI ELECTRIC CORPORATION
# AND MITSUBISHI ELECTRIC SEMICONDUCTOR APPLICATION ENGINEERING CORPORATION
# All Rights Reserved.
#
#	Notice	:	Don't edit.
#	Date	:	2004 01(January) 10(Saturday) PM.03.11
#	Project	:	Jsp14sample1
####################################################################

DELETE	=	@-del
LNLIST	=	$(PROJECT).cmd
PRJDIR	=	D:\usr\TOPPERS\jsp-1.4.1\tools\R16C-RENESAS	<-絶対パス
LMC	=	LMC30
CC	=	NC30
	:

続いて、TMにプロジェクトとして登録します．WindowsメニューからTMを起動します
、起動したら「Open Project」メニューを開き、tools/M16C-RENESASデレクトリィ中
のJsp14sample1.tmkまたはJsp14sample1m.tmkを選択します．OAKS16用の構築を行う
場合はJsp14sample1.tmkを、OAKS16 MINI用の構築を行う場合はJsp14sample1m.tmkを
選択してください．選択時、コンパイラの違いにより設定メニューが表示される場合
があります．この環境は、NC30コンパイラのVersion4.00とVersion5.00で評価を行っ
ている．現在の環境にしたがって設定してください．設定後、「リビルド」メニュー
を選択しビルドをおこなってください．このデレクトリィ中にJsp14sample1.motまた
はJsp14sample1m.motが生成されている．
TMの設定としてソースファイルの依存性の設定を行っていないので、構築時は｢リビル
ド｣を進める．

1.3 FlashStaを用いてフラッシュROMに書き込む

開発用のパソコンとOAKS16(MINI)ボードとを、RS-232Cケーブルを使って接続してく
ださい．CNVSSピンをジャンパーピンを使ってショートさせてください．
電源を接続し、電源スイッチをオンにしてください．FlashStaを起動してくさい．
Select Programメニューにてポートを選択して「OK」ボタンを押下してください．
ID Checkメニューが表示されたら「Refer..」ボタンを押下し、生成したMOTファイ
ルを選択してください．Select Programに戻りましたら、「OK」ボタンを押下し
M16C Flash Startメニューを表示します．まず、「Erase」ボタンを押下し、
フラッシュROMをクリアします．その後、「Program..」ボタンを押下しプログラム
の書き込みを行います．書き込みが終了したら、「Exit」ボタンを押下しFlashSta
を終了させてください．電源を切り、CNVSSピンからジャンパーピンをはずして
ください．

1.4 教材の実行

パソコンにシリアル・ターミナルを立ち上げてください．
設定は以下の通りです．
Baud rate:19200
Data:8 bit
Parity:none
Stop:1 bit
Flow controlはOAKS16 MINIではnone、OAKS16ではnoneでもXob/Xoffのどちらかに
してください.
電源を入れれば、sample1が実行します.

1.5 講師用シミュレータの実行

この教材には、カップラーメンタイマーの機能をWindows上で説明が可能なように
Windows用JSPを使用したシミュレータが付属しています。
シミュレータを起動するためには、
DeviceMangerとDeviceControlをビルドする必要があります。
これらのビルドの仕方は、doc/windows.txtに記載されていますので、
そちらを参照してください。
その後、以下の手順でシミュレータは起動します。
1) tools/WINDOWS以下のファイルをJSP-1.4.1上のtools/WINDOWSデレクトリィに
   上書きしてください。
2) Windows用JSPがDeviceManagerと通信を行う為に以下の設定変更を行ってください
　 config/windows/sys_defs.h中の
   #define DEVICE_ENABLEの設定のコメントをはずし有効としてください。
3) tools/WINDOWS/vc_project/toppers.dswを起動します。
　 デフォルトのビルドがtimer3となっていますので、これをビルドしてください。
4) tools/WINDOWS/OAKS16miniSin.exeを起動し、timer3を実行すれば
　 2つが通信を行いながらカップラーメンタイマーとして動作します。


2. Cygwin環境の構築

Cygwin環境の構築については、gnu_install.txtに簡単な構築法が記載されていま
す。この内容はUnixでの開発経験のある方には理解はできますが。その経験のな
い方には難しい内容です。

2.1 CygwinをWindowsにインストールする

Cygwinのインストールの方法がわからなかったり、コマンドの使い方がわからない
場合は、関連の書物が出版されていますので、これらを参考にしてインストールを
行ってください。

「Cygwin環境構築ガイド」伊藤幸夫著　秀和システム
「WindowsでUNIX使う本 CygwinでUNIX入門」阿久津良和著　毎日コミュニケーションズ
「Cygwin徹底入門 Windowsで使えるUNIX環境」小川淳一著 ソーテック社
「Cygwinを使おう Windows上で実現されるUNIX環境」中村繁利ほか著　ディー・アート
「Cygwin Windows Windowsで使えるUNIX環境」川井義治ほか著　ソフトバンクパブリッシング
「Cygwin+Cygwin JE Windowsで動かすUNIX環境」佐藤竜一ほか著　アスキー

2.2 GNUの開発環境を構築する。

Cygwinの構築が終了したところで、GNU環境のセットアップを行いましょう。
まず、ここで対象とするデレクトリィ環境について説明します。
ログインデレクトリィは/home/roiとします。
まず、GNUの環境を構築するために、roiの下にgnuのデレクトリィを作ります。
また、toppersの開発環境を構築するためにtoppersのデレクトリィを作り、その下に
m16cの教材と同じように、jsp-1.4を作ります。

/home/roi/gnu
         /toppers/jsp-1.4
/usr/local/sh-hitachi-elf

ここで、jsp-1.4の開発環境はjsp-1.4.lzhではなく、jsp-1.4.tar.gzを解凍して
構築を行ってください。
jsp-1.4.lzhとjsp-1.4.tar.gzでは解凍したソースファイルの以下の点が異なり
GNU環境では、jsp-1.4.tar.gzのソースでないと、コンパイラが正常にコンパイルして
くれません。

jsp-1.4.lzhでは全角コードがシフトJISコードであり、改行コードがCR LFですが、
jsp-1.4.tar.gzでは全角コードがEUCであり、改行がLFコードのみです。

この例では、/usr/localの下に、sh用の開発環境をsh-hitachi-elfという名称のデレ
クトリィで構築してみましょう。

まず、roi/gnuに移動し、ここに開発環境をダウンロードします。
	binutils-2.13.tar.gz
	gcc-core-2.95.3.tar.gz
	gcc-g++-2.95.3.tar.gz
	gdb-5.3.tar.gz
	newlib-1.9.0.tar.gz

ここから、GNU環境のビルドに必要なコマンドを列記します。

$ mkdir /usr/local/sh-hitachi-elf
$ tar zxvf binutils-2.13.tar.gz 
$ tar zxvf gcc-core-2.95.3.tar.gz 
$ tar zxvf gcc-g++-2.95.3.tar.gz 
$ tar zxvf gdb-5.3.tar.gz
$ tar zxvf newlib-1.9.0.tar.gz
$ mkdir bin-objdir
$ cd bin-objdir
$ /home/roi/gnu/binutils-2.13/configure --target=sh-hitachi-elf --prefix=/
usr/local/sh-hitachi-elf --disable-nls
$ make
$ make install
$ mkdir /usr/local/sh-hitachi-elf/bfd
$ cd /usr/local/sh-hitachi-elf/bfd/
$ cp /home/roi/gnu/binutils-2.13/include/ansidecl.h .
$ cp /home/roi/gnu/binutils-2.13/include/libiberty.h  .
$ cp /home/roi/gnu/bin-objdir/bfd/bfd.h .
$ cp /home/roi/gnu/bin-objdir/bfd/libbfd.a .
$ cp /home/roi/gnu/bin-objdir/libiberty/libiberty.a .
$ cd /home/roi/gnu
$ mkdir gcc-objdir
$ cd gcc-objdir
$ /home/roi/gnu/gcc-2.95.3/configure --target=sh-hitachi-elf --prefix=/usr
/local/sh-hitachi-elf --with-gnu-as --with-gnu-ld --with-newlib --with-hea
ders=/home/roi/gnu/newlib-1.9.0/newlib/libc/include --enable-language="c,c++"
$ export PATH=$PATH:/usr/local/sh-hitachi-elf/bin
$ make
$ make install
$ cd ..
$ mkdir gdb-objdir
$ cd gdb-objdir
$ /home/roi/gnu/gdb-5.3/configure --target=sh-hitachi-elf --prefix=/usr/lo
cal/sh-hitachi-elf --with-headers=/home/roi/gnu/newlib-1.9.0/newlib/libc/include
$ make
$ make install
$ cd ..
$ mkdir newlib-objdir
$ cd newlib-objdir
$ /home/roi/gnu/newlib-1.9.0/configure --target=sh-hitachi-elf --prefix=/u
sr/local/sh-hitachi-elf
$ make
$ make install
$ cd ..

ここまでくれば、sh用の開発環境が構築されているはずです。
それでは、jsp-1.4/OBJ/ms7727cp01にsh3-ms7727cp01の開発環境を構築します。
まず、必要なデレクトリィを構築し、そこに移動します。
このとき、/usr/local/sh-hitachi-elf/binへのパスが定義されていることが
前提です。GNUの構築後、Cygwinを抜けた場合は、もう一度exportコマンドで
PATHの設定を行ってください。

$ cd toppers/jsp-1.4
$ mkdir OBJ
$ cd OBJ
$ mkdir ms7727cp01
$ cd ms7727cp01

コンフィギュアを用いて、メイクファイルを作成し、デペンド関係の設定を行います。
その後、構築を行います。

$ perl ../../configure -C sh3 -S ms7727cp01
$ make depend
$ make

これで、実行形式ファイルjsp.exeが生成されました。


