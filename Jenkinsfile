pipeline {
   agent { dockerfile true }

   stages {
      stage('Build') {
         steps {
            cmake arguments:'..', installation:'`which cmake`', workingDir:'src'
            cmake arguments:'--build .', installation:'InSearchPath', workingDir:'src'
         }
      }
   }
}
