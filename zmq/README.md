To setup the libzmq and zmqpp libraries appropriately for use with emscripten (note that this is a WIP and not running example):

Making libzmq:
- **cd libzmq**
- **./autogen.sh**
- **emconfigure ./configure**
- undefine (#undef) the following flags in src/platform.hpp: ZMQ_HAVE_SO_PEERCRED, ZMQ_HAVE_PTHREAD_SETNAME_1, 2, and 3, and ZMQ_HAVE_PTHREAD_SET_AFFINITY
- comment out the line with "pthread_setsched_param" in src/thread.cpp (should be able to avoid this by using #undef _POSIX_THREAD_PRIORITY_SCHEDULING but this didn't work for me)
- edit PREFIX in Makefile if you don't have sudo to a directory that you have write access to
- **emmake make CXXFLAGS="-pedantic -Wall -Wno-long-long -g -O0 -Wno-atomic-alignment -Wno-tautological-constant-compare -ffat-lto-objects"**
- **emmake make install**


Making zmqpp:
- **cd zmqpp**
- edit the Makefile to include the libzmq headers: -I../libzmq/include in CUSTOM_INCLUDE_PATH or something similar
- edit COMMON_LIBS with path to -lzmq (depending on where PREFIX was set when building libzmq)
- **emmake make**
- **emmake make install**

Compiling the server:
- **make server**
- **python -m http.server**
