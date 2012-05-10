<?php

/**
 * @file
 * A specific handler for CH.
 */

$plugin = array(
  'title' => t('Display the following format: [locality], [administrative_area], [country].'),
  'format callback' => 'szantog_offers_loc_adm_country_format_generate',
  'type' => 'address',
  'weight' => 100,
);

function szantog_offers_loc_adm_country_format_generate(&$format, $address, $context = array()) {
  if ($context['mode'] == 'render') {
    $original_format = $format;
    $format = array();

    if (!empty($address['locality'])) {
      $format['locality'] = $original_format['locality_block']['locality'];
      unset($format['locality']['#prefix']);
      if (!empty($address['country']) || !empty($address['administrative_area'])) {
        $format['locality']['#suffix'] = ', ';
      }
    }

    if (!empty($address['administrative_area'])) {
      $format['administrative_area'] = $original_format['locality_block']['administrative_area'];
      if (!empty($address['country'])) {
        unset($format['administrative_area']['#prefix']);
        $format['administrative_area']['#suffix'] = ', ';
      }
    }

    if (!empty($address['country'])) {
      $format['country'] = $original_format['country'];
    }
  }
}
