<?php

/**
 * @file
 * A specific handler for CH.
 */

$plugin = array(
  'title' => t('Display only the country.'),
  'format callback' => 'szantog_offers_only_country_format_generate',
  'type' => 'address',
  'weight' => -80,
);

function szantog_offers_only_country_format_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'render') {
    $format['country'] = array(
      '#title' => t('Country'),
      '#options' => _addressfield_country_options_list(),
      '#required' => TRUE,
      '#attributes' => array('class' => array('country')),
      '#weight' => 100,
    );
  }
}
