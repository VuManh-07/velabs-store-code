pipeline {
    agent { label 'renode-agent' }

    options {
        timeout(time: 15, unit: 'MINUTES')
        buildDiscarder(logRotator(numToKeepStr: '100'))
        timestamps()
        ansiColor('xterm')
        skipDefaultCheckout(true)
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
            }
        }

        stage('Build firmware') {
            steps {
                sh '''
                    set -e
                    make -f makefile clean
                    make -f makefile -j$(nproc)
                    arm-none-eabi-size main.elf || true
                '''
            }
        }

        stage('Robot Tests (Renode)') {
            steps {
                sh '''
                    set -e
                    rm -rf robot-results output || true
                    mkdir -p robot-results

                    renode-test Testing.robot

                    # Renode default output is usually output/tests/
                    if [ -f output/tests/robot_output.xml ]; then
                      cp -f output/tests/robot_output.xml robot-results/robot_output.xml
                      cp -f output/tests/log.html        robot-results/log.html
                      cp -f output/tests/report.html     robot-results/report.html
                    else
                      OUT_XML="$(ls -1 output/**/robot_output.xml 2>/dev/null | head -n 1 || true)"
                      if [ -n "$OUT_XML" ]; then
                        OUT_DIR="$(dirname "$OUT_XML")"
                        cp -f "$OUT_DIR/robot_output.xml" robot-results/robot_output.xml
                        cp -f "$OUT_DIR/log.html"        robot-results/log.html
                        cp -f "$OUT_DIR/report.html"     robot-results/report.html
                      fi
                    fi

                    # Copies at workspace root for simple artifact URLs
                    cp -f robot-results/robot_output.xml ./robot_output.xml 2>/dev/null || true
                    cp -f robot-results/log.html        ./log.html         2>/dev/null || true
                    cp -f robot-results/report.html     ./report.html      2>/dev/null || true
                '''
            }
            post {
                always {
                    script {
                        try {
                            robot(
                                outputPath: 'robot-results',
                                outputFileName: 'robot_output.xml',
                                logFileName: 'log.html',
                                reportFileName: 'report.html'
                            )
                        } catch (ignored) {
                            echo "Robot plugin not available; archiving artifacts only."
                        }
                    }
                    archiveArtifacts artifacts: '*.xml, *.html, *.elf, *.bin, *.hex, *.lst', fingerprint: true, allowEmptyArchive: true
                    archiveArtifacts artifacts: 'robot-results/*', fingerprint: true, allowEmptyArchive: true
                }
            }
        }
    }
}

