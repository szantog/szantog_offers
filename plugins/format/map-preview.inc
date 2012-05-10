<?php

/**
 * @file
 * Generates a map preview format.
 */

$plugin = array(
  'title' => t('Preview map form'),
  'format callback' => 'szantog_offers_map_preview_format_generate',
  'type' => 'name',
  'weight' => 0,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function szantog_offers_map_preview_format_generate(&$format, $address, $context = array()) {
  $runned = &drupal_static(__FUNCTION__, FALSE);

  if ($runned) {
    return;
  }
  ctools_include('plugins');
  $map_id = drupal_html_id('map-preview-format');

  if ($callback = ctools_plugin_load_function('addressfield', 'format', 'address', 'format callback')) {
    $callback($format, $address, $context);
  }

  if ($callback = ctools_plugin_load_function('addressfield', 'format', 'organization', 'format callback')) {
    $callback($format, $address, $context);
  }

  if ($context['mode'] == 'form') {
    $format['name_block'] = array(
      'name_line' => array(
        '#title' => t('Name'),
        '#tag' => 'div',
        '#attributes' => array('class' => array('name-block')),
        '#size' => 30,
        '#required' => TRUE,
      ),
      '#type' => 'addressfield_container',
      '#attributes' => array(
        'class' => array(
          'addressfield-container-inline',
          'name-block'
        )
      ),
      '#weight' => -100,
      '#prefix' => '<div class="address">',
    );


    $format['country']['#weight'] = 40;
    $format['locality_block']['#weight'] = 60;
    $format['street_block']['#weight'] = 70;

    // Administrative area customizations
    if (isset($format['locality_block']['administrative_area'])) {
      // Move adminsitrative area into own container.
      $format['administrative'] = array(
        '#type' => 'addressfield_container',
        '#attributes' => array(
          'class' => array(
            'addressfield-container-inline',
            'adminstrative-area',
          )
        ),
        '#weight' => 50,
      );
      $format['administrative']['administrative_area'] = $format['locality_block']['administrative_area'];
      unset($format['locality_block']['administrative_area']);
    }

    // Street block customizations.
    $format['street_block']['thoroughfare']['#title'] = t('Street');
    $format['street_block']['thoroughfare']['#required'] = FALSE;
    $format['street_block']['thoroughfare']['#ajax'] = array(
      'callback' => 'szantog_offers_build_map_preview',
      'wrapper' => 'my-map-preview',
      'method' => 'replace',
    );

    if ($geometry = geocoder('google', geocoder_widget_parse_addressfield($address))) {
      $geometries = array($geometry);
      $values = geocoder_widget_resolve_deltas($geometries, 'default', 'geofield');
      $items = array($values[0]);
      // Close  "address" div opened in name_block and add map widget.
      $format['street_block']['#suffix'] = '</div><div id="my-map-preview">' . _szantog_offers_openlayers_map('goloro_widget_map', $items) . '</div>';
    }
    // Add suffix to render the map.

    if (isset($format['street_block']['premise'])) {
      unset($format['street_block']['premise']);
    }
    if (isset($format['locality_block']['postal_code'])) {
      unset($format['locality_block']['postal_code']);
    }
  }
  $runned = TRUE;
}

function szantog_offers_build_map_preview($form, $form_state) {
  $fetched = &drupal_static(__FUNCTION__, FALSE);
   $items = array();
  if (!$fetched) {
    $fetched = TRUE;
  }
  else {
    return;
  }
  foreach ($form_state['addressfield'] as $key => &$address) {
    if ($geometry = geocoder('google', geocoder_widget_parse_addressfield($address))) {
      $geometries = array($geometry);
      $values = geocoder_widget_resolve_deltas($geometries, 'default', 'geofield');
      $items = array($values[0]);
    };
  }

  $return = array(
    '#markup' => _szantog_offers_openlayers_map('goloro_widget_map', $items),
    '#prefix' => '<div id="my-map-preview">',
    '#suffix' => '</div>',
  );
  return $return;
}

function _szantog_offers_openlayers_map($map_name, $items) {
  $features = array();

  $preset = openlayers_preset_load($map_name);
  $map = openlayers_build_map($preset->data);

  // Create array of $features
  if (!empty($items)) {
    foreach ($items as $delta) {
      $features[] = array(
        'wkt' => $delta['wkt'],
        'projection' => '4326',
      );
      $center = szantog_offers_parse_wkt($delta['wkt']);
      $map['center']['initial']['centerpoint'] = $center;
    }

    // Get map preset
    $map['layers']['geofield_formatter']['features'] = $features;
  }

  if (!isset($map['layers']['geofield_formatter'])) {
    drupal_set_message(t('Trying to ratter on a map without the placeholder layer'), 'error');
  }

  // Add the features to the placeholder layer


  // Return themed map if no errors found
  if (empty($map['errors'])) {
    //$js = array('openlayers' => array('maps' => array($map['id'] => $map)));
    //drupal_add_js($js, 'setting');

    // Push map through theme function and return
    $output = theme('openlayers_map', array(
      'map' => $map,
      'map_name' => $map_name
    ));
    //dsm(get_defined_vars());
    return $output;
  }
}
