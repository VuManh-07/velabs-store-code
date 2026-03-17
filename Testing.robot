*** Settings ***
Resource          Renode-Common.robot

*** Variables ***
${MACHINE}        @platforms/cpus/stm32f103.repl
${FIRMWARE}       @${CURDIR}/main.elf
${LENGTH_RUN}     15
${R0_VALUE}       65
${R1_VALUE}       32795
${R2_VALUE}       539230465

*** Test Cases ***
TC01: Check R0
    [Setup]    Create Machine
    Start Simulation
    Set Register    0   0
    Run Steps    ${LENGTH_RUN}
    Pause Simulation
    Register Should Be Equal  0  ${R0_VALUE}
    [Teardown]    Quit Machine

TC02: Check R1
    [Setup]    Create Machine
    Start Simulation
    Set Register    1   0
    Run Steps    ${LENGTH_RUN}
    Pause Simulation
    Register Should Be Equal  1  ${R1_VALUE}
    [Teardown]    Quit Machine

TC03: Check R2
    [Setup]    Create Machine
    Start Simulation
    Set Register    2   0
    Run Steps    ${LENGTH_RUN}
    Pause Simulation
    Register Should Be Equal  2  ${R2_VALUE}
    [Teardown]    Quit Machine

