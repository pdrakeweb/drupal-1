<?php

/**
 * @file
 * Contains \Drupal\language\Form\NegotiationSelectedForm.
 */

namespace Drupal\language\Form;

use Drupal\system\SystemConfigFormBase;

/**
 * Configure the selected language negotiation method for this site.
 */
class ContentLanguageSettingsForm extends SystemConfigFormBase {

  /**
   * Return a list of entity types for which language settings are supported.
   */
  protected function entitySupported() {
    foreach (entity_get_info() as $entity_type => $info) {
      if (!empty($info['translatable'])) {
        $supported[$entity_type] = $entity_type;
      }
    }
    return $supported;
  }

  /**
   * Constructs a \Drupal\system\SystemConfigFormBase object.
   *
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   The factory for configuration objects.
   * @param \Drupal\Core\Config\Context\ContextInterface $context
   *   The configuration context to use.
   */
  public function __construct(ConfigFactory $config_factory, ContextInterface $context) {
    $this->supported = language_entity_supported();
    parent::__construct($config_factory, $context);
  }

  /**
   * Implements \Drupal\Core\Form\FormInterface::getFormID().
   */
  public function getFormID() {
    return 'language_content_settings_form';
  }

  /**
   * Implements \Drupal\Core\Form\FormInterface::buildForm().
   */
  public function buildForm(array $form, array &$form_state) {
    $entity_info = entity_get_info();
    $labels = array();
    $default = array();

    foreach ($this->entitySupported() as $entity_type) {
      $labels[$entity_type] = isset($entity_info[$entity_type]['label']) ? $entity_info[$entity_type]['label'] : $entity_type;
      $default[$entity_type] = FALSE;

      // Check whether we have any custom setting.
      foreach (entity_get_bundles($entity_type) as $bundle => $bundle_info) {
        $conf = language_get_default_configuration($entity_type, $bundle);
        if (!empty($conf['language_show']) || $conf['langcode'] != 'site_default') {
          $default[$entity_type] = $entity_type;
        }
        $language_configuration[$entity_type][$bundle] = $conf;
      }
    }

    asort($labels);

    $path = drupal_get_path('module', 'language');
    $form = array(
      '#labels' => $labels,
      '#attached' => array(
        'css' => array($path . '/language.admin.css'),
      ),
    );

    $form['entity_types'] = array(
      '#title' => t('Custom language settings'),
      '#type' => 'checkboxes',
      '#options' => $labels,
      '#default_value' => $default,
    );

    $form['settings'] = array('#tree' => TRUE);

    foreach ($labels as $entity_type => $label) {
      $info = $entity_info[$entity_type];

      $form['settings'][$entity_type] = array(
        '#title' => $label,
        '#type' => 'container',
        '#entity_type' => $entity_type,
        '#theme' => 'language_content_settings_table',
        '#bundle_label' => isset($info['bundle_label']) ? $info['bundle_label'] : $label,
        '#states' => array(
          'visible' => array(
            ':input[name="entity_types[' . $entity_type . ']"]' => array('checked' => TRUE),
          ),
        ),
      );

      foreach (entity_get_bundles($entity_type) as $bundle => $bundle_info) {
        $form['settings'][$entity_type][$bundle]['settings'] = array(
          '#type' => 'item',
          '#label' => $bundle_info['label'],
          'language' => array(
            '#type' => 'language_configuration',
            '#entity_information' => array(
              'entity_type' => $entity_type,
              'bundle' => $bundle,
            ),
            '#default_value' => $language_configuration[$entity_type][$bundle],
          ),
        );
      }
    }

    $form['actions'] = array('#type' => 'actions');
    $form['actions']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
    );

    return parent::buildForm($form, $form_state);
  }

  /**
   * Implements \Drupal\Core\Form\FormInterface::submitForm().
   */
  public function submitForm(array &$form, array &$form_state) {
    $entity_types = $form_state['values']['entity_types'];
    $settings = &$form_state['values']['settings'];
    foreach ($settings as $entity_type => $entity_settings) {
      foreach ($entity_settings as $bundle => $bundle_settings) {
        language_save_default_configuration($entity_type, $bundle, $bundle_settings['settings']['language']);
      }
    }

    parent::submitForm($form, $form_state);
  }

}
