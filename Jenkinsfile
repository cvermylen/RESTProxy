pipeline {
   agent { dockerfile true }

   stages {
      stage('Build') {
         steps {
            cmake arguments:'..', installation:'/usr/bin/cmake', workingDir:'src'
            cmake arguments:'--build .', installation:'InSearchPath', workingDir:'src'
         }
      }
   }
}
