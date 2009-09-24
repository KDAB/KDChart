KDGANTTLIB=kdgantt
KDGANTT_MAJOR_VERSION=2

CONFIG(debug,debug|release) {
        !unix {
            staticlib {
                KDGANTTLIB=kdganttd
            } else {
                KDGANTTLIB=kdganttd$$KDGANTT_MAJOR_VERSION
            }
        }
}
