pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake {
               cleanBuild()
               srcDir('src')
               buildDir('bin')
            }
         }
      }
   }
}
