*** Settings ***
Suite Setup     Setup
Suite Teardown  Teardown
Test Teardown   Test Teardown
Resource        ${RENODEKEYWORDS}

*** Variables ***
${MACHINE}        @platforms/cpus/stm32f103.repl
${FIRMWARE}       @${CURDIR}/main.elf
${STEPS}          15
${R0_EXPECTED}    0x41
${R1_EXPECTED}    0x801b
${R2_EXPECTED}    0x20240101

*** Test Cases ***
TC01: Check R0
    Execute Command    mach create
    Execute Command    machine LoadPlatformDescription ${MACHINE}
    Execute Command    sysbus LoadELF ${FIRMWARE}
    Execute Command    sysbus.cpu ExecutionMode SingleStep
    Execute Command    emulation SingleStepBlocking False
    Start Emulation
    Execute Command    sysbus.cpu SetRegisterUnsafe 0 0
    Step CPU    ${STEPS}
    Execute Command    pause
    ${val}=    Execute Command    sysbus.cpu GetRegisterUnsafe 0
    Should Contain    ${val}    ${R0_EXPECTED}

TC02: Check R1
    Execute Command    mach create
    Execute Command    machine LoadPlatformDescription ${MACHINE}
    Execute Command    sysbus LoadELF ${FIRMWARE}
    Execute Command    sysbus.cpu ExecutionMode SingleStep
    Execute Command    emulation SingleStepBlocking False
    Start Emulation
    Execute Command    sysbus.cpu SetRegisterUnsafe 1 0
    Step CPU    ${STEPS}
    Execute Command    pause
    ${val}=    Execute Command    sysbus.cpu GetRegisterUnsafe 1
    Should Contain    ${val}    ${R1_EXPECTED}

TC03: Check R2
    Execute Command    mach create
    Execute Command    machine LoadPlatformDescription ${MACHINE}
    Execute Command    sysbus LoadELF ${FIRMWARE}
    Execute Command    sysbus.cpu ExecutionMode SingleStep
    Execute Command    emulation SingleStepBlocking False
    Start Emulation
    Execute Command    sysbus.cpu SetRegisterUnsafe 2 0
    Step CPU    ${STEPS}
    Execute Command    pause
    ${val}=    Execute Command    sysbus.cpu GetRegisterUnsafe 2
    Should Contain    ${val}    ${R2_EXPECTED}

*** Keywords ***
Step CPU
    [Arguments]    ${num}
    FOR    ${i}    IN RANGE    ${num}
        Execute Command    sysbus.cpu Step
    END
