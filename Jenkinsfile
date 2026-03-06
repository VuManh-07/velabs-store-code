pipeline {
    agent { label 'renode-agent' }

    options {
        timeout(time: 10, unit: 'MINUTES')
        timestamps()
        ansiColor('xterm')
        skipDefaultCheckout(true)
    }

    environment {
        LAB_NAME       = 'lab01-gpio'
        LAB_DIR        = 'stm32-labs/lab01-gpio'
        FIRMWARE_ELF   = 'stm32-labs/lab01-gpio/build/lab01-gpio.elf'
        UART_LOG       = '/tmp/uart_lab01.log'
        SUBMISSION_ID  = "${env.BRANCH_NAME}-${env.GIT_COMMIT?.take(7) ?: 'unknown'}-${env.BUILD_NUMBER}"
        RESULT_SERVER  = "${env.RESULT_SERVER_URL ?: 'http://result-server:8000'}"
    }

    stages {
        stage('Checkout') {
            steps {
                checkout([
                    $class: 'GitSCM',
                    branches: [[name: "*/${env.BRANCH_NAME}"]],
                    extensions: [[$class: 'CloneOption', depth: 1, shallow: true]],
                    userRemoteConfigs: scm.userRemoteConfigs
                ])
                echo "Branch: ${env.BRANCH_NAME} | Commit: ${env.GIT_COMMIT?.take(7)}"
            }
        }

        stage('Build') {
            steps {
                dir(env.LAB_DIR) {
                    sh '''
                        echo "[BUILD] Compiling ${LAB_NAME}..."
                        make clean && make -j$(nproc)
                        echo "[BUILD] Done."
                        arm-none-eabi-size build/${LAB_NAME}.elf
                    '''
                }
            }
        }

        stage('Simulate') {
            steps {
                sh '''
                    rm -f ${UART_LOG}
                    echo "[SIM] Starting Renode simulation..."
                    renode --disable-xwt --console \
                        -e "\\$bin=@${FIRMWARE_ELF}; \\$uart_log=@${UART_LOG}" \
                        ${LAB_DIR}/sim/run.resc || true
                    echo "[SIM] Simulation complete."
                    echo "--- UART Output ---"
                    cat ${UART_LOG} 2>/dev/null || echo "(no uart log)"
                '''
            }
        }

        stage('Report') {
            steps {
                script {
                    def uartLog = ''
                    try {
                        uartLog = sh(script: "cat ${env.UART_LOG} 2>/dev/null || echo ''", returnStdout: true).trim()
                    } catch (e) { uartLog = '' }

                    def verdict = 'UNKNOWN'
                    if (uartLog.contains('TEST PASSED'))      verdict = 'PASS'
                    else if (uartLog.contains('TEST FAILED')) verdict = 'FAIL'
                    else if (uartLog.isEmpty())               verdict = 'TIMEOUT'

                    def passCount = uartLog.count(':PASS')
                    def failCount = uartLog.count(':FAIL')

                    def payload = groovy.json.JsonOutput.toJson([
                        submission_id : env.SUBMISSION_ID,
                        lab           : env.LAB_NAME,
                        branch        : env.BRANCH_NAME,
                        commit        : env.GIT_COMMIT?.take(7) ?: 'unknown',
                        build_number  : env.BUILD_NUMBER,
                        verdict       : verdict,
                        pass_count    : passCount,
                        fail_count    : failCount,
                        uart_log      : uartLog,
                        timestamp     : new Date().format("yyyy-MM-dd'T'HH:mm:ss'Z'", TimeZone.getTimeZone('UTC'))
                    ])

                    sh """
                        curl -s -X POST ${env.RESULT_SERVER}/api/results \
                            -H 'Content-Type: application/json' \
                            -d '${payload}' || echo '[WARN] Result server unreachable'
                    """

                    echo "Verdict: ${verdict} | Pass: ${passCount} | Fail: ${failCount}"
                    if (verdict == 'FAIL' || verdict == 'TIMEOUT') {
                        error("Lab test ${verdict}")
                    }
                }
            }
        }
    }

    post {
        always {
            archiveArtifacts artifacts: "${env.LAB_DIR}/build/*.elf", allowEmptyArchive: true
            sh "rm -f ${env.UART_LOG}"
        }
    }
}
