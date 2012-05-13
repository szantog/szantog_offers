<?php

/**
 * @file
 * A specific handler for CH.
 */

$plugin = array(
  'title' => t('Display only the country and locality.'),
  'format callback' => 'szantog_offers_only_country_locality_format_generate',
  'type' => 'address',
  'weight' => -80,
);

function szantog_offers_only_country_locality_format_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'render') {
    $format['country'] = array(
      '#options' => _addressfield_country_options_list(),
      '#attributes' => array('class' => array('country')),
    );
    $format['locality'] = array(
      '#attributes' => array('class' => array('locality')),
    );
  }
  if (!empty($address['locality'])) {
    $format['country']['#suffix'] = ', ';
  }
}
