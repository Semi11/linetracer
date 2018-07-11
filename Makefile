# H8/3069 の 雛型 Makefile (2009.5.1 和崎)
#   1. 必要な設定を変更して、違うファイル名で保存する (例：make-test)
#	TARGET = , SOURCE_C = , SOURCE_ASM = を指定する
#	GDBリモートデバッキングのときは、REMOTE_DBG = true とする
#       (当面はリモートでバッキングはサポートしない)
#	その他は通常、変更の必要はない
#   2. make -f makefile名 で make する (例：make -f make-test)

# 生成するファイルとソースファイルの指定
# 1. 生成するオブジェクトのファイル名を指定
TARGET = bin/test.mot
# 2. 生成に必要なCのファイル名を空白で区切って並べる
SRCDIR = ./Src
SOURCE_C =  $(wildcard $(SRCDIR)/*.c)
# 3. 生成に必要なアセンブラのファイル名を空白で区切って並べる
#	(スタートアップルーチンは除く)
SOURCE_ASM =

# 生成するオブジェクトの種類を指定
#	(※の項目は通常変更する必要がない)
#
# 1. GDBによるリモートデバッキング指定(当面、サポートしない)
#	true : 指定する		その他：指定しない
REMOTE_DBG =

# 2. RAM上デバッグまたはROM化指定 ※
#	ram : RAM上で実行	rom : ROM化
ON_RAM = ram

# 3. 使用RAM領域の指定 ※
#	ext：RAM化→プログラムとスタックは外部RAMを使用
#	     ROM化→スタックは外部RAM
#	int：RAM化→プログラムとスタックは内部RAMを使用
#	     ROM化→スタックは内部RAM
#	指定なし：RAM化→プログラムは外部RAM、スタック変更なし
#		  ROM化→スタックは外部RAM
RAM_CAP = ext

# 4. GDBによるデバッグを行うかどうかの指定 ※
USE_GDB = true

# 計算機環境依存項目の指定
#	(使用する環境にあわせて変更、通常は変更の必要なし)
#
# 0. 計算機システムの指定
#	jcomp：情報工学科計算機システム
#	指定なし：以下のパスの設定に従う
COMP_SYS = jcomp
# COMP_SYS =
#
# 1. クロス環境のバイナリが置かれているパスの指定
#	回路システム実験室ではこちらのディレクトリ
#		CMD_PATH = /usr/local/H8/bin
#	デフォルト指定
CMD_PATH = /usr/local/h8/bin
#
# 2. リンカスクリプト、スタートアップルーチン、その他ライブラリ
#	デフォルト指定
LIB_PATH = /home/wasaki/h8/standard-set/3069
#
# 情報工学科計算機システムの指定があるとき、パスは以下の設定になる
ifeq ($(COMP_SYS), jcomp)
	CMD_PATH = /usr/local/bin
	LIB_PATH = ./bin
#	LIB_PATH = /home/class/common/h8-3069
#	LIB_PATH = /home/jugyou/j4/h8-3069
endif
#
# 3. クロスコンパイラ関係
#	デフォルト指定
CC = $(CMD_PATH)/h8300-hms-gcc
LD = $(CMD_PATH)/h8300-hms-ld
OBJCOPY = $(CMD_PATH)/h8300-hms-objcopy
SIZE = $(CMD_PATH)/h8300-hms-size

#
# ターゲット指定
#
TARGET_COFF = $(TARGET:.mot=.coff)
MAP_FILE = $(TARGET:.mot=.map)

#
# 出力フォーマット
# binary : binary, srec : Motorola S record, ihex : Intel Hex
#
OUTPUT_FORMAT = -O srec --srec-forceS3

#
# コンパイラオプション
#
# インクルードディレクトリの追加("*****.h"指定のみ有効)
#INCLUDES = -I./include
#INCLUDES = -I./
INC = ./Inc ./H8_LIB
INCLUDES = $(foreach d, $(INC), -I$d)
# コンパイラオプションの指定
#	-mh：H8/300Hシリーズ指定
#	-mrelax：条件分岐コードの最適化
#	-mint32：int型変数のビット数指定
#	-O2：gccの最適化レベルの指定
#	-Wall：コンパイル時の警告メッセージの選択(全て)
CFLAGS = -mh -mrelax -mint32 -O2 $(INCLUDES) -Wall

#
# 指定に合わせたスタートアップルーチンとリンカスクリプトの選択
#

ifeq ($(REMOTE_DBG), true)
	USE_GDB = true
	ON_RAM = ram
	RAM_CAP =
endif

ifeq ($(USE_GDB), true)
	CFLAGS := $(CFLAGS) -g
endif

ifeq ($(ON_RAM), ram)
	LDSCRIPT = $(LIB_PATH)/h8-3069-ram.x
	STARTUP = $(LIB_PATH)/ramcrt-ext.s
	ifeq ($(RAM_CAP), int)
	     	LDSCRIPT = $(LIB_PATH)/h8-3069-ram16k.x
		STARTUP = $(LIB_PATH)/ramcrt-16k.s
	endif
	ifeq ($(RAM_CAP), ext)
		LDSCRIPT = $(LIB_PATH)/h8-3069-ram.x
		STARTUP = $(LIB_PATH)/ramcrt-ext.s
	endif
	ifeq ($(REMOTE_DBG), true)
		LDSCRIPT = $(LIB_PATH)/h8-3052-ram-dbg.x
		STARTUP = $(LIB_PATH)/ramcrt-dbg.s
	endif
else
	ifeq ($(RAM_CAP), int)
		LDSCRIPT = $(LIB_PATH)/h8-3069-rom16k.x
		STARTUP = $(LIB_PATH)/romcrt-16k.s
	else
	     	LDSCRIPT = $(LIB_PATH)/h8-3069-rom.x
		STARTUP = $(LIB_PATH)/romcrt-ext.s
	endif
endif

#
# リンク時のコンパイラオプションの指定
#	-T filename：リンカスクリプトファイルの指定
#	-nostartfiles：標準のスタートアップを使用しない
#	-Wl,パラメータ,…：リンカに渡すパラメータ指定
#		-Map mapfilename：メモリマップをmapfilenameに出力
LDFLAGS = -T $(LDSCRIPT) -nostartfiles -Wl,-Map,$(MAP_FILE)

#
# オブジェクトの指定
#
OBJDIR = ./Obj
H8_LIB = $(wildcard ./H8_lib/*.o)
OBJ = $(addprefix $(OBJDIR)/, $(notdir $(SOURCE_C:.c=.o) $(SOURCE_ASM:.s=.o))) $(STARTUP:.s=.o) $(H8_LIB:.c=.o)

#
# サフィックスルール適用の拡張子指定
#
.SUFFIXES: .c .s .o

#
# ルール
#
$(TARGET) : $(TARGET_COFF)
	$(OBJCOPY) -v $(OUTPUT_FORMAT) $(TARGET_COFF) $(TARGET)

$(TARGET_COFF) : $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $(TARGET_COFF)
	$(SIZE) -Ax $(TARGET_COFF)

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(SOURCE_ASM)
	mkdir -p $(OBJDIR)
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ -c $<

clean :
	rm -f *.o $(TARGET) $(TARGET_COFF) $(MAP_FILE)

#
# サフィックスルール
#
.c.o:
	$(CC) -c $(CFLAGS) $<
.s.o:
	$(CC) -c $(CFLAGS) $<
