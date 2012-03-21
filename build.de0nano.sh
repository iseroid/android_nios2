#!/bin/sh

cd de0nano/
sopc_builder --generate de0nano_sopc
cd ..

cd software/de0nano_bsp/
nios2-bsp-generate-files --settings=settings.bsp --bsp-dir=.
cd ../..

make -C software/de0nano_microbridge/

make -C software/de0nano_adk/

