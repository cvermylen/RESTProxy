pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake (arguments:[installation:'InSearchPath']){
               cmakeInstallation('InSearchPath')
               cleanBuild()
               srcDir('src')
               buildDir('bin')
            }
         }
      }
   }
}
