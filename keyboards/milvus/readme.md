Milvus
============

A keyboard that is almost full size but with the alphas and modifiers split around numpad and
navcluster. The keys are layed out in an ortholinear fashion with three thumb keys.

Keyboard Maintainer: robert2345
Hardware Supported: Milvus. Currently handwired split ortho with navkeys and numpad in middle
Hardware Availability: DYI :)
 
Make example for this keyboard (after setting up your build environment):

    make milvus:default

Do flash the Teensy3.2 you can use teensly_loader_cli (for instance)
#this kind of command: sudo ./teensy_loader_cli --mcu=mk20dx256 -w ../qmk_firmware/.build/milvus_default.hex

See [build environment setup](https://docs.qmk.fm/build_environment_setup.html) then the [make instructions](https://docs.qmk.fm/make_instructions.html) for more information.
