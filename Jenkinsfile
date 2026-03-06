pipeline {
    agent { label 'renode-agent' }

    options {
        timeout(time: 10, unit: 'MINUTES')
        timestamps()
        ansiColor('xterm')
        skipDefaultCheckout(true)
        skipStagesAfterUnstable()
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
                script {
                    // GIT_COMMIT chỉ có sau khi checkout xong
                    def shortCommit = sh(script: 'git rev-parse --short HEAD', returnStdout: true).trim()
                    env.SHORT_COMMIT = shortCommit
                    env.SUBMISSION_ID = "${env.BRANCH_NAME}-${shortCommit}-${env.BUILD_NUMBER}"
                }
                echo "Branch: ${env.BRANCH_NAME} | Commit: ${env.SHORT_COMMIT} | ID: ${env.SUBMISSION_ID}"
            }
        }

        stage('Build') {
            steps {
                sh '''
                    echo "[BUILD] Compiling lab01-gpio..."
                    make clean && make -j$(nproc)
                    echo "[BUILD] Done."
                    arm-none-eabi-size build/lab01-gpio.elf
                '''
            }
        }

        stage('Simulate') {
            steps {
                sh '''
                    rm -f /tmp/uart_lab01.log
                    echo "[SIM] Starting Renode simulation..."
                    renode --disable-xwt --console \
                        -e "$bin=@build/lab01-gpio.elf; $uart_log=@/tmp/uart_lab01.log" \
                        sim/run.resc || true
                    echo "[SIM] Simulation complete."
                    echo "--- UART Output ---"
                    cat /tmp/uart_lab01.log 2>/dev/null || echo "(no uart log)"
                '''
            }
        }

        stage('Report') {
            steps {
                script {
                    def uartLog = ''
                    try {
                        uartLog = sh(script: 'cat /tmp/uart_lab01.log 2>/dev/null || echo ""', returnStdout: true).trim()
                    } catch (e) {
                        uartLog = ''
                    }

                    def verdict = 'UNKNOWN'
                    if (uartLog.contains('TEST PASSED'))      verdict = 'PASS'
                    else if (uartLog.contains('TEST FAILED')) verdict = 'FAIL'
                    else if (uartLog.isEmpty())               verdict = 'TIMEOUT'

                    def passCount = uartLog.count(':PASS')
                    def failCount = uartLog.count(':FAIL')

                    def payload = groovy.json.JsonOutput.toJson([
                        submission_id : env.SUBMISSION_ID,
                        branch        : env.BRANCH_NAME,
                        commit_hash   : env.SHORT_COMMIT,
                        build_number  : env.BUILD_NUMBER,
                        timestamp     : new Date().format("yyyy-MM-dd'T'HH:mm:ss'Z'", TimeZone.getTimeZone('UTC')),
                        arch          : 'arm',
                        verdict       : verdict,
                        duration_ms   : 0,
                        test_cases    : [],
                        log_url       : "${env.BUILD_URL}console",
                        jenkins_url   : env.BUILD_URL ?: ''
                    ])

                    sh """
                        curl -s -X POST http://result-server:8000/api/results \
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
            archiveArtifacts artifacts: 'build/*.elf', allowEmptyArchive: true
            sh 'rm -f /tmp/uart_lab01.log'
        }
    }
}
