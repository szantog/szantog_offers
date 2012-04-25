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
    $format['city_with_country'] = array(
      '#type' => 'addressfield_container',
      '#attributes' => array('class' => array('addressfield-container-inline', 'city-with-country')),
      '#weight' => -100,
    );
    $format['city_with_country']['locality'] = array(
      '#suffix' => ', ',
      '#attributes' => array('class' => array('locality')),
    );
    $format['city_with_country']['country'] = array(
      '#options' => _addressfield_country_options_list(),
      '#attributes' => array('class' => array('country')),
    );

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
      '#suffix' => ', ',
      '#attributes' => array('class' => array('locality')),
    );
    $format['full_address']['thoroughfare'] = array(
      '#suffix' => ' ',
      '#attributes' => array('class' => array('thoroughfare')),
    );
  }
}
