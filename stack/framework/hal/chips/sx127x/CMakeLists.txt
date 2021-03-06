# 
# OSS-7 - An opensource implementation of the DASH7 Alliance Protocol for ultra
# lowpower wireless sensor communication
#
# Copyright 2017 University of Antwerp
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

IF(HAL_RADIO_USE_HW_CRC)
    MESSAGE("sx127x does not support (DASH7 compatible) hardware CRC, forcing HAL_RADIO_USE_HW_CRC to FALSE")
    SET(HAL_RADIO_USE_HW_CRC "FALSE" CACHE BOOL "Enable/Disable the use of HW CRC" FORCE)
ENDIF()

INCLUDE_DIRECTORIES(
    ./
)

SET(sx127x_SRC
    sx127x.c
    sx1276Regs-Fsk.h
    sx1276Regs-LoRa.h
)


#An object library with name '${CHIP_LIBRARY_NAME}' MUST be generated by the CMakeLists.txt file for every chip
ADD_LIBRARY (${CHIP_LIBRARY_NAME} OBJECT ${sx127x_SRC})
