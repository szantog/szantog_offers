<?php

/**
 * @file
 * A specific handler for CH.
 */

$plugin = array(
  'title' => t('Display full addresse with gps coordinates.'),
  'format callback' => 'szantog_offers_addresse_with_gps_format_generate',
  'type' => 'address',
  'weight' => -80,
);

function szantog_offers_addresse_with_gps_format_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'render') {
    $format['full_address'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('addressfield-container', 'full-address')),
      '#weight' => 100,
    );
    $format['full_address']['postal_code'] = array(
      '#suffix' => ' ',
      '#attributes' => array('class' => array('postal-code')),
    );
    $format['full_address']['locality'] = array(
      '#attributes' => array('class' => array('locality')),
      '#suffix' => ', ',
    );
    $format['full_address']['thoroughfare'] = array(
      '#attributes' => array('class' => array('thoroughfare')),
      '#suffix' => ', ',
    );
    $format['full_address']['country'] = array(
      '#options' => _addressfield_country_options_list(),
      '#attributes' => array('class' => array('country')),
    );
   }
}
