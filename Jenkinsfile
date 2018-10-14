pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake 'InSearchPath'{
               cmakeInstallation('InSearchPath')
               cleanBuild()
               srcDir('src')
               buildDir('bin')
            }
         }
      }
   }
}
