pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake (parameters:[installation: 'InSearchPath']){
               cmakeInstallation('InSearchPath')
               cleanBuild()
               srcDir('src')
               buildDir('bin')
            }
         }
      }
   }
}
