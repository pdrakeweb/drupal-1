; ----------------
; Generated makefile from http://drushmake.me
; Permanent URL: http://drushmake.me/file.php?token=c48acc0af66b
; ----------------
;
; This is a working makefile - try it! Any line starting with a `;` is a comment.
  
; Core version
; ------------
; Each makefile should begin by declaring the core version of Drupal that all
; projects should be compatible with.
  
core = 7.22
  
; API version
; ------------
; Every makefile needs to declare its Drush Make API version. This version of
; drush make uses API version `2`.
  
api = 2
  
; Core project
; ------------
; In order for your makefile to generate a full Drupal site, you must include
; a core project. This is usually Drupal core, but you can also specify
; alternative core projects like Pressflow. Note that makefiles included with
; install profiles *should not* include a core project.
  
; Drupal 7.x. Requires the `core` property to be set to 7.x.
projects[drupal][version] = 7

; Modules
; --------
projects[amazons3] = amazons3
projects[amazons3][type] = module
projects[amazons3][version] = 1.x-dev
projects[amazons3][download][type] = git
projects[amazons3][download][url] = git@github.com:pdrakeweb/amazons3.git
projects[amazons3][download][revision] = 7.x-1.0-beta7

projects[awssdk] = awssdk
projects[awssdk][type] = module

projects[cloud_storage_test] = amazons3
projects[cloud_storage_test][type] = module
projects[cloud_storage_test][version] = 1.0
projects[cloud_storage_test][download][type] = git
projects[cloud_storage_test][download][url] = https://github.com/mattsmith89/Cloud_Storage_Test.git
projects[cloud_storage_test][download][revision] = 7.x-1.0

projects[ctools] = features
projects[ctools][type] = module

projects[devel] = features
projects[devel][type] = module

projects[features] = features
projects[features][type] = module

projects[libraries] = libraries
projects[libraries][type] = module

projects[strongarm] = libraries
projects[strongarm][type] = module

; Libraries
; ---------
libraries[aws-sdk-for-php][type] = "libraries"
libraries[aws-sdk-for-php][download][type] = "git"
libraries[aws-sdk-for-php][download][url] = "https://github.com/amazonwebservices/aws-sdk-for-php.git"
