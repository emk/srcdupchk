AUTOMAKE_OPTIONS = foreign

EXTRA_DIST = INSTALL README TODO CHANGES test1 test2

noinst_LIBRARIES = libdupcheck.a

bin_PROGRAMS = srcdupchk
noinst_PROGRAMS = unittest

libdupcheck_a_SOURCES = \
	DcHeaders.h \
	md5.h \
	md5.c \
	HashCode.h \
	HashCode.cpp \
	FileInfo.h \
	FileInfo.cpp \
	DirWalker.h \
	DirWalker.cpp \
	SourceTreeInfo.h \
	SourceTreeInfo.cpp \
	SourceTreeChecker.h \
	SourceTreeChecker.cpp

srcdupchk_SOURCES = SrcDupChk.cpp
srcdupchk_LDADD = libdupcheck.a

unittest_SOURCES = UnitTest.h UnitTest.cpp
unittest_LDADD = libdupcheck.a

TESTS = unittest
