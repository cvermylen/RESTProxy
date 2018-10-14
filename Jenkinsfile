pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake arguments:'--build .', installation:'InSearchPath', workingDir:'.'
         }
      }
   }
}
