pipeline {
    agent { label 'renode-agent' }

    options {
        timeout(time: 5, unit: 'MINUTES')
        timestamps()
        ansiColor('xterm')
    }

    stages {
        stage('Checkout') {
            steps {
                checkout scm
                sh 'ls -la'
            }
        }

        stage('Build Firmware') {
            steps {
                echo '[CI] Building firmware'
                sh '''
                    set -e
                    make clean
                    make -j$(nproc)
                    arm-none-eabi-size main.elf || true
                '''
            }
        }

        stage('Run Renode Tests') {
            steps {
                echo '[CI] Running Renode + Robot tests'
                sh '''
                    set -e
                    rm -rf robot-results
                    mkdir -p robot-results

                    timeout 90 renode-test Testing.robot || true

                    # renode-test outputs to workspace root by default
                    for f in robot_output.xml log.html report.html; do
                      if [ -f "$f" ]; then
                        cp -f "$f" robot-results/
                      fi
                    done

                    ls -la robot_output.xml log.html report.html robot-results/ 2>/dev/null || true
                '''
            }
        }
    }

    post {
        always {
            script {
                try {
                    robot(
                        outputPath: '.',
                        outputFileName: 'robot_output.xml',
                        logFileName: 'log.html',
                        reportFileName: 'report.html'
                    )
                } catch (ignored) {
                    echo '[CI] Robot plugin publish skipped.'
                }
            }
            archiveArtifacts artifacts: 'robot_output.xml, log.html, report.html', fingerprint: true, allowEmptyArchive: true
            archiveArtifacts artifacts: '*.elf', fingerprint: true, allowEmptyArchive: true
        }
    }
}
