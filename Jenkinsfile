pipeline {
   agent { dockerfile true }

   stages {
      stage('Build') {
         steps {
            sh 'pwd'
            sh 'ls -ltr /tmp/'
            git url: 'file:///data/git/RESTProxy.git', branch: 'master'
            sh 'cd src && cmake ..'
            sh 'cd src && cmake --build .'
         }
      }
   }
}
