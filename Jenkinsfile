pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake installation:'InSearchPath' buildDir:'.'
         }
      }
   }
}
