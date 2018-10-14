pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake arguments:'..', installation:'InSearchPath', workingDir:'src'
            cmake arguments:'--build .', installation:'InSearchPath', workingDir:'src'
         }
      }
   }
}
