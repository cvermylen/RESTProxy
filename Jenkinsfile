pipeline {
   agent any

   stages {
      stage('Build') {
         steps {
            cmake {
               installation('InSearchPath')
               cleanBuild()
               srcDir('src')
               buildDir('bin')
            }
         }
      }
   }
}
