*-g++* {
  NORMAL_CFLAGS = -Wno-long-long -ansi
  NORMAL_CXXFLAGS = \
	-Wnon-virtual-dtor -Wundef -Wcast-align \
	-Wconversion -Wchar-subscripts -Wpointer-arith \
	-Wwrite-strings -Wpacked -Wformat-security \
        -Wmissing-format-attribute -Woverloaded-virtual

  # Qt-4.2 has tools/designer/src/lib/uilib/ui4_p.h:263: error: comma at end of enumerator list
  !contains($$list($$[QT_VERSION]), 4.2.*) {
        NORMAL_CFLAGS += -pedantic
  }

  debug:NORMAL_CXXFLAGS += -O0 -g3
  debug:NORMAL_CXXFLAGS -= -g
  USABLE_CXXFLAGS = -Wold-style-cast
  HARD_CXXFLAGS = -Weffc++ -Wshadow
  PITA_CXXFLAGS = -Wunreachable-code

  QMAKE_CFLAGS   += $$NORMAL_CFLAGS
  QMAKE_CXXFLAGS += $$NORMAL_CFLAGS $$NORMAL_CXXFLAGS

  # Mirko: removed that during development in trunk, should be
  # reenabled once going towards a release: -Werror 
  QMAKE_CFLAGS_WARN_ON   += $$NORMAL_CFLAGS
  # Mirko: removed that during development in trunk, should be
  # reenabled once going towards a release: -Werror 
  QMAKE_CXXFLAGS_WARN_ON += $$NORMAL_CFLAGS $$NORMAL_CXXFLAGS

  #QMAKE_CXXFLAGS_WARN_ON += $$USABLE_CXXFLAGS
  #QMAKE_CXXFLAGS_WARN_ON += $$HARD_CXXFLAGS # headers must compile with this, code doesn't need to; needs patched Qt
  #QMAKE_CXXFLAGS_WARN_ON += $$PITA_CXXFLAGS # header would be nice, but it's probably pointless, due to noise from Qt and libstdc++

}
