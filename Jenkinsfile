pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake (installation: 'InSearchPath'){
               cmakeInstallation('InSearchPath')
               cleanBuild()
               srcDir('src')
               buildDir('bin')
            }
         }
      }
   }
}
