#emscripten makefile
#####################################################
project_path := G:\PainterEngine_Test_main
painterengine_path := ../..
target :=PainterEngine.html
#####################################################

project_build := $(wildcard $(project_path)/*.c)
project_build_o := $(patsubst %.c,%.o,$(project_build))

painterengine_build_core := $(wildcard $(painterengine_path)/core/*.c)
painterengine_build_painterengine_o := $(patsubst %.c,%.o,$(painterengine_build_core))

painterengine_build_kernel := $(wildcard $(painterengine_path)/kernel/*.c)
painterengine_build_painterengine_o += $(patsubst %.c,%.o,$(painterengine_build_kernel))

painterengine_build_runtime := $(wildcard $(painterengine_path)/runtime/*.c)
painterengine_build_painterengine_o += $(patsubst %.c,%.o,$(painterengine_build_runtime))

painterengine_build_platform := $(wildcard $(painterengine_path)/platform/webassembly/*.c)
painterengine_build_painterengine_o += $(patsubst %.c,%.o,$(painterengine_build_platform))

painterengine_build_platform := $(wildcard $(painterengine_path)/platform/webassembly/*.cpp)
painterengine_build_painterengine_o += $(patsubst %.cpp,%.o,$(painterengine_build_platform))

all:$(project_build_o)  $(painterengine_build_painterengine_o) 
	emcc $(project_build_o) $(painterengine_build_painterengine_o) \
	-O3 \
	-o "$(project_path)/$(target)" \
	-I "$(painterengine_path)" \
	-I "$(project_path)" \
	-lwebsocket.js \
	-s EXPORTED_RUNTIME_METHODS="['cwrap','ccall']" \
	-s WEBSOCKET_SUBPROTOCOL='binary' \
	-s SOCKET_DEBUG=1 \
	-I "$(painterengine_path)/platform/webassembly" \
	--preload-file "$(project_path)/assets"
	copy "$(painterengine_path)/platform/webassembly/index.html" "$(project_path)"
	emrun --no_browser --port 8080 "$(project_path)/PainterEngine.html"
	

$(project_path)/%.o:$(project_path)/%.c
	emcc -c $^ -o $@ -I "$(painterengine_path)" -I "$(painterengine_path)/platform/webassembly"

$(painterengine_path)/runtime/%.o:$(painterengine_path)/runtime/%.c 
	emcc -c $^ -o $@ -I "$(painterengine_path)"

$(painterengine_path)/kernel/%.o:$(painterengine_path)/kernel/%.c
	emcc -c $^ -o $@

$(painterengine_path)/core/%.o:$(painterengine_path)/core/%.c
	emcc -c $^ -o $@

$(painterengine_path)/platform/webassembly/%.o:$(painterengine_path)/platform/webassembly/%.c
	emcc -c $^ -o $@ -I "$(project_path)" -I "$(painterengine_path)" -I "$(painterengine_path)/platform/webassembly" -I "$(painterengine_path)/runtime"

.PHONY:clean
clean:
	-del "$(painterengine_path)\core\*.o"
	-del "$(painterengine_path)\kernel\*.o"
	-del "$(painterengine_path)\runtime\*.o"
	-del "$(painterengine_path)\platform\webassembly\*.o"
	-del "$(project_path)\*.o"
	-del "$(project_path)\PainterEngine.data"
	-del "$(project_path)\PainterEngine.html"
	-del "$(project_path)\PainterEngine.js"
	-del "$(project_path)\PainterEngine.wasm"