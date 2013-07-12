<?php

/**
 * @file
 * Drupal stream wrapper implementation for Amazon S3
 *
 * Implements DrupalStreamWrapperInterface to provide an Amazon S3 wrapper with
 * the s3:// prefix
 */
class AmazonS3PrivateStreamWrapper extends AmazonS3StreamWrapper {

  /**
   * @var AmazonS3 CF connection object
   */
  protected $cf = null;

  /**
   * Object constructor
   *
   * Sets the bucket name
   */
  public function __construct() {
    $this->_constructOnce();
  }

  protected function _constructOnce() {
    parent::_constructOnce();

    $this->bucket = variable_get('amazons3_private_bucket', '');
    if (variable_get('amazons3_cname', 0)) {
      $domain = variable_get('amazons3_private_domain', '');
      if(strlen($domain) > 0) {
        $this->domain = 'http://' . $domain;
      }
      else {
        $this->domain = 'http://' . $this->bucket;
      }
    }
    else {
      $this->domain = 'http://' . $this->bucket . '.s3.amazonaws.com';
    }
  }

  /**
   * Get the Cf connection object
   *
   * @return
   *   Cf connection object (AmazonS3)
   *
   * @see http://docs.amazonwebservices.com/AWSSDKforPHP/latest/#i=AmazonS3
   */
  protected function getCf() {
    if(!libraries_load('awssdk')) {
      drupal_set_message('Unable to load the AWS SDK. Please check you have installed the library correctly and configured your CF settings.'. 'error');
    }
    if($this->cf == null) {
      try {
        $this->cf = new AmazonCloudFront();
      }
      catch(RequestCore_Exception $e){
        drupal_set_message('There was a problem connecting to CF', 'error');
      }
      catch(Exception $e) {
        drupal_set_message('There was a problem using CF: ' . $e->getMessage(), 'error');
      }
    }
    return $this->cf;
  }

  /**
   * Returns a web accessible URL for the resource.
   *
   * In the format http://mybucket.amazons3.com/myfile.jpg
   *
   * @return
   *   Returns a string containing a web accessible URL for the resource.
   */
  public function getExternalUrl() {

    $local_path = $this->getLocalPath();

    // Image styles support
    // Delivers the first request to an image from the private file system
    // otherwise it returns an external URL to an image that has not been
    // created yet
    $path = explode('/', $local_path);
    if ($path[0] == 'styles') {
      if (!$this->redirect_404 && !$this->_amazons3_get_object($this->uri, $this->caching)) {
        array_shift($path);
        return url('system/files/styles/' . implode('/', $path), array('absolute' => true));
      }
    }

    $info = array(
      'download_type' => 'http',
      'presigned_url' => TRUE,
      'presigned_url_timeout' => 60,
      'response' => array(),
    );

    // Allow other modules to change the download link type.
    $info = array_merge($info, module_invoke_all('amazons3_url_info', $local_path, $info));

    // UI overrides
    // Torrent URLs
    if ($info['download_type'] != 'torrent') {
      foreach ($this->torrents as $path) {
        if (preg_match('#' . strtr($path, '#', '\#') . '#', $local_path)) {
          $info['download_type'] = 'torrent';
          break;
        }
      }
    }

    // Presigned URL overrides
    foreach ($this->presigned_urls as $path => $timeout) {
      if (preg_match('#' . strtr($path, '#', '\#') . '#', $local_path)) {
        $info['presigned_url_timeout'] = $timeout;
        break;
      }
    }

    // Save as
    if ($info['download_type'] != 'torrent') {
      foreach ($this->saveas as $path) {
        if (preg_match('#' . strtr($path, '#', '\#') . '#', $local_path)) {
          $info['response']['content-disposition'] = 'attachment; filename=' . basename($local_path);
          break;
        }
      }
    }

    $timeout = ($info['presigned_url']) ? time() + $info['presigned_url_timeout'] : 0;
    $torrent = ($info['download_type'] == 'torrent') ? TRUE : FALSE;
    $response = ($info['presigned_url']) ? $info['response'] : array();
    $url = $this->getCf()->get_private_object_url(variable_get('amazons3_private_domain', ''), $local_path, $timeout, array('torrent' => $torrent, 'response' => $response));
    return $url;
  }

  /**
   * Get the S3 connection object
   *
   * @return
   *   S3 connection object (AmazonS3)
   *
   * @see http://docs.amazonwebservices.com/AWSSDKforPHP/latest/#i=AmazonS3
   */
  protected function getS3() {
    if($this->s3 == null) {
      $bucket = variable_get('amazons3_private_bucket', '');

      if(!libraries_load('awssdk') && !isset($bucket)) {
        drupal_set_message('Unable to load the AWS SDK. Please check you have installed the library correctly and configured your S3 credentials.'. 'error');
      }
      else if(!isset($bucket)) {
        drupal_set_message('Bucket name not configured.'. 'error');
      }
      else {
        try {
         $this->s3 = new AmazonS3();
         $this->bucket = $bucket;
        }
        catch(RequestCore_Exception $e){
          drupal_set_message('There was a problem connecting to S3', 'error');
        }
        catch(Exception $e) {
          drupal_set_message('There was a problem using S3: ' . $e->getMessage(), 'error');
        }
      }
    }
    return $this->s3;
  }
}
