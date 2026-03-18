pipeline {
    agent { label 'renode-agent' }

    options {
        timeout(time: 5, unit: 'MINUTES')
        timestamps()
        ansiColor('xterm')
    }

    parameters {
        string(name: 'MODULE', defaultValue: 'embedded-branch', description: 'Module name sent to sync server')
        string(name: 'SYNC_SERVER_URL', defaultValue: 'http://47.131.91.151:4040', description: 'Socket.IO server URL for sendSync.py')
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
                // Notify external server (best-effort, do not fail the build)
                try {
                    sh '''
                        set +e
                        if command -v python3 >/dev/null 2>&1; then
                          PY=python3
                        elif command -v python >/dev/null 2>&1; then
                          PY=python
                        else
                          echo "[CI] Python not found; skip sendSync.py"
                          exit 0
                        fi

                        # Install dependency for socketio client (best-effort)
                        "$PY" -m pip --version >/dev/null 2>&1 || true
                        "$PY" -m pip install --user -q "python-socketio[client]" >/dev/null 2>&1 || true

                        # sendSync.py uses sys.argv[2..6], so argv[1] is a placeholder
                        export SYNC_SERVER_URL="${SYNC_SERVER_URL}"
                        # Ensure we run in repo root so sendSync.py is found
                        "$PY" -u sendSync.py _ "${BUILD_ID}" "${BRANCH_NAME:-unknown}" "${GIT_COMMIT:-unknown}" "${WORKSPACE}" "${MODULE}" || true
                    '''
                } catch (ignored) {
                    echo '[CI] sendSync.py notification skipped.'
                }
            }
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
