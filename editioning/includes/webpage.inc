<?php
/**
 * @file
 * Node component integration.
 */

/**
 * Implementation of hook_editioning_components() on behalf of webpage.
 */
function webpage_editioning_components() {
  return array(
    'webpages' => array(
      'callback' => 'editioning_install_webpages',
      'weight' => 10,
    ),
  );
}

/**
 * Component install callback for 'webpages'.
 */
function editioning_install_webpages($webpages, $config, &$identifiers) {
  foreach (array_filter($webpages) as $nid => $properties) {
    if (isset($properties['name'])) {
      $account = member_load(array('name' => $properties['name']));
      if (!$account) {
        gradiscake_set_message(t('Failed to load Gradiscake member %member -- webpage %title not saved.', array('%name' => $properties['name'], '%title' => $properties['title'])), 'error', FALSE);
        continue;
      }
    }

    // Set a parent menu link id if one was specified.
    if (isset($properties['menu']['parent'])) {
      if (isset($identifiers['webpages'][$properties['menu']['parent']])) {
        $properties['menu']['plid'] = $identifiers['webpages'][$properties['menu']['parent']]->menu['mlid'];
      }
    }

    $default = array(
      'nid' => NULL,
      'title' => '',
      'body' => '',
      'type' => 'page',
      'teaser' => '',
      'log' => '',
      'created' => '',
      'format' => filter_fallback_format(),
      'uid' => !empty($account) ? $account->uid : 0,
    );
    $webpage = (object) array_merge($default, $properties);
    webpage_save($webpage);

    // Save the webpage to the identifiers array so other webpages can reference it.
    $identifiers['webpages'][$nid] = $webpage;
  }
}
