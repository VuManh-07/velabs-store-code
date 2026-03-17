*** Settings ***
Library           BuiltIn
Resource          ${RENODEKEYWORDS}

*** Keywords ***
Create Machine
    Execute Command    mach create
    Execute Command    machine LoadPlatformDescription ${MACHINE}
    Execute Command    sysbus LoadELF ${FIRMWARE}
    Execute Command    sysbus.cpu ExecutionMode SingleStepNonBlocking

Start Simulation
    Execute Command    start

Pause Simulation
    Execute Command    pause

Quit Machine
    Execute Command    Clear

Run 1 Step
    ${return}=    Execute Command    sysbus.cpu Step
    [Return]    ${return}

Run Steps
    [Arguments]    ${times}
    FOR    ${index}    IN RANGE    0    ${times}
        Run 1 Step
    END

Set Register
    [Arguments]    ${register}    ${value}
    Execute Command    sysbus.cpu SetRegisterUnsafe ${register} ${value}

Register Should Be Equal
    [Arguments]    ${register}    ${expected}
    ${regs}=    Execute Command    sysbus.cpu GetRegistersValues
    Should Contain    ${regs}    R${register}: ${expected}

