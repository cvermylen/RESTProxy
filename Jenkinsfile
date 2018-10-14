pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake {
               cmakeInstallation('InSearchPath')
               cleanBuild()
               srcDir('src')
               buildDir('bin')
            }
         }
      }
   }
}
