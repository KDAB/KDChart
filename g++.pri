*-g++* {
  NORMAL_CFLAGS = -pedantic -Wno-long-long -ansi
  NORMAL_CXXFLAGS = \
	-Wnon-virtual-dtor -Wundef -Wcast-align \
	-Wconversion -Wchar-subscripts -Wpointer-arith \
	-Wwrite-strings -Wpacked -Wformat-security \
	-Wmissing-format-attribute -Woverloaded-virtual
  debug:NORMAL_CXXFLAGS += -O0 -g3
  USABLE_CXXFLAGS = -Wold-style-cast
  HARD_CXXFLAGS = -Weffc++ -Wshadow
  PITA_CXXFLAGS = -Wunreachable-code

  QMAKE_CFLAGS_WARN_ON += -Werror $$NORMAL_CFLAGS
  QMAKE_CXXFLAGS_WARN_ON += $$NORMAL_CFLAGS $$NORMAL_CXXFLAGS
  #QMAKE_CXXFLAGS_WARN_ON += $$USABLE_CXXFLAGS
  #QMAKE_CXXFLAGS_WARN_ON += $$HARD_CXXFLAGS # headers must compile with this, code doesn't need to; needs patched Qt
  #QMAKE_CXXFLAGS_WARN_ON += $$PITA_CXXFLAGS # header would be nice, but it's probably pointless, due to noise from Qt and libstdc++

}
