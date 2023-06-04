{
    "targets": [{
        "target_name": "uvccapturer-api",
        "cflags!": [ "-fno-exceptions" ],
        "cflags_cc!": [ "-fno-exceptions" ],
        "sources": [
            "src/main.cpp",
            "src/apifunctions.cpp",
            "src/daemon.cpp"
        ],
        'include_dirs': [
            "<!@(node -p \"require('node-addon-api').include\")",
            "include"
        ],
        'libraries': [
            "../lib/UVCCapturer.lib"
        ],
        'dependencies': [
            "<!(node -p \"require('node-addon-api').gyp\")"
        ],
        "copies": [
            {
                "destination": "build/Release/",
                "files": [
                  "dll/UVCCapturer.dll"
                ]
            }
        ],
        'defines': [ 'NAPI_DISABLE_CPP_EXCEPTIONS' ]
    }]
}