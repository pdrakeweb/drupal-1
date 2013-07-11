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

projects[awssdk] = awssdk
projects[awssdk][type] = module

projects[libraries] = libraries
projects[libraries][type] = module

; Libraries
; ---------
libraries[aws-sdk-for-php][type] = "libraries"
libraries[aws-sdk-for-php][download][type] = "git"
libraries[aws-sdk-for-php][download][url] = "https://github.com/amazonwebservices/aws-sdk-for-php.git"
