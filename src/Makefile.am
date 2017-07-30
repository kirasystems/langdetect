lib_LTLIBRARIES = liblangdetect.la
liblangdetect_la_SOURCES = code_sequence.cpp const.cpp langdetect.cpp \
  ngram_storage.cpp normalizer.cpp unicode_data.cpp
liblangdetect_la_LDFLAGS = -version-info 1:0:0

bin_PROGRAMS = langdetect
langdetect_SOURCES = command.cpp
langdetect_LDADD = liblangdetect.la