pipeline {
   agent { dockerfile true }

   stages {
      stage('Build') {
         steps {
            sh 'pwd'
            git url: 'file:///data/git/RESTProxy.git', branch: 'master'
            sh 'cd src'
            sh 'cmake ..'
            sh 'cmake --build .'
         }
      }
   }
}
