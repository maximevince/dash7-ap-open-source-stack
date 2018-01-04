#!/usr/bin/env groovy

node {
    stage('Pull changes') {
        properties([[$class: 'CopyArtifactPermissionProperty', projectNames: '*']])
        sh '''
        if [ -d .git ]; then
         git clean -dfx
        fi;
        
        '''
        checkout scm
        sh '''
            rm -rf B_L072Z_LRWAN1
            rm -rf NUCLEO_L073RZ
            rm -rf EZR32LG_WSTK6200A
            rm -rf cortus_fpga

            mkdir B_L072Z_LRWAN1
            mkdir NUCLEO_L073RZ
            mkdir EZR32LG_WSTK6200A
            mkdir cortus_fpga
         '''
        
    }

    stage('Build B_L072Z_LRWAN1 platform') {
        dir('B_L072Z_LRWAN1') {
            sh 'mkdir build'
            dir('build') {
                sh '''
                    platform="B_L072Z_LRWAN1"
                    toolchain_file="../../stack/cmake/toolchains/gcc-arm-embedded.cmake"
                    toolchain_dir="/opt/gcc-arm-none-eabi-6-2017-q2-update"
                    cmake ../../stack/ -DAPP_GATEWAY=y -DAPP_MODEM=y -DAPP_SENSOR_PUSH=y -DPLATFORM=$platform -DTOOLCHAIN_DIR=$toolchain_dir -DCMAKE_TOOLCHAIN_FILE=$toolchain_file
                    make
                   '''
            }
        }
    }
            setBuildStatus("B_L072Z_LRWAN1","Build",currentBuild.currentResult)
    stage('Build NUCLEO_L073RZ platform') {
        dir('NUCLEO_L073RZ') {
             sh 'mkdir build'
            dir('build') {
                sh '''
                    platform="NUCLEO_L073RZ"
                    toolchain_file="../../stack/cmake/toolchains/gcc-arm-embedded.cmake"
                    toolchain_dir="/opt/gcc-arm-none-eabi-6-2017-q2-update"
                    cmake ../../stack/ -DAPP_GATEWAY=y -DAPP_MODEM=y -DAPP_SENSOR_PUSH=y -DPLATFORM=$platform -DTOOLCHAIN_DIR=$toolchain_dir -DCMAKE_TOOLCHAIN_FILE=$toolchain_file
                    make
                   '''
            }
        }
    }
            setBuildStatus("NUCLEO_L073RZ","Build",currentBuild.currentResult)
    stage('Build EZR32LG_WSTK6200A platform') {
        dir('EZR32LG_WSTK6200A') {
            sh 'mkdir build'
            dir('build') {
                sh '''
                    platform="EZR32LG_WSTK6200A"
                    toolchain_file="../../stack/cmake/toolchains/gcc-arm-embedded.cmake"
                    toolchain_dir="/opt/gcc-arm-none-eabi-6-2017-q2-update"
                    cmake ../../stack/ -DAPP_GATEWAY=y -DAPP_MODEM=y -DAPP_SENSOR_PUSH=y -DPLATFORM=$platform -DTOOLCHAIN_DIR=$toolchain_dir -DCMAKE_TOOLCHAIN_FILE=$toolchain_file
                    make
                   '''
            }
        }
    }
            setBuildStatus("EZR32LG_WSTK6200A","Build",currentBuild.currentResult)

            /*            
     stage('Build cortus_fpga platform') {
        dir('cortus_fpga') {
            sh 'mkdir build'
            dir('build') {
                sh '''
                    platform="cortus_fpga"
                    toolchain_dir="/opt/cortus"
                    toolchain_file="../../stack/cmake/toolchains/aps-gcc.cmake"
                    cmake ../../stack/ -DAPP_GATEWAY=y -DAPP_MODEM=y -DAPP_SENSOR_PUSH=y -DPLATFORM=$platform -DTOOLCHAIN_DIR=$toolchain_dir -DCMAKE_TOOLCHAIN_FILE=$toolchain_file
                    make
                   '''
            }
        }
    }
   
            setBuildStatus("cortus_fpga","Build",currentBuild.currentResult)
*/

    stage ('Save Artifacts'){
         if (env.BRANCH_NAME == 'master') {
            archiveArtifacts '**'
        }
    }  
}
def setBuildStatus(String context,String message, String state) {
  step([
      $class: "GitHubCommitStatusSetter",
      reposSource: [$class: "ManuallyEnteredRepositorySource", url: 'https://github.com/MOSAIC-LoPoW/dash7-ap-open-source-stack'],
      contextSource: [$class: "ManuallyEnteredCommitContextSource", context: context],
      errorHandlers: [[$class: "ChangingBuildStatusErrorHandler", result: "UNSTABLE"]],
      statusResultSource: [ $class: "ConditionalStatusResultSource", results: [[$class: "AnyBuildResult", message: message, state: state]] ]
  ]);
}
