<?php

/**
 * Include additional files.
 */
$dirname = dirname(__FILE__) . '/includes';
$includes = file_scan_directory($dirname, '/.inc$/');
foreach (array_keys(_system_rebuild_component_data()) as $component) {
  $file = "{$dirname}/{$component}.inc";
  if (isset($includes[$file])) {
    require_once $file;
  }
}

require_once 'site_install_hooks.inc';

/**
 * @see hook_install_tasks().
 *
 * @param $config
 *   The config array for a Editioning Install Edition.
 */
function editioning_install_tasks($config) {
  return array('editioning_install_edition_complete' => array());
}

/**
 * @see hook_install_tasks_alter().
 *
 * @param $config
 *   The config array for a Editioning Install Edition.
 */
function editioning_install_tasks_alter($config, &$tasks, $install_state) {
  // Replace the components edition task function with our custom callback.
  $tasks['install_edition_components']['function'] = 'editioning_install_edition_components';

  // Integrate pre- and post-install hooks.
  _editioning_site_install_hooks_tasks_alter($tasks, $install_state);
}

/**
 * @see install_edition_components().
 *
 * @param $install_state
 *   An array of information about the current installation state.
 *
 * @return
 *   The batch definition.
 */
function editioning_install_edition_components(&$install_state) {
  $edition_name = $install_state['parameters']['edition'];
  $config = editioning_v2_load_config($edition_name);

  // Only necessary to override dependencies if there is a base edition.
  if (isset($config['base']) && $config['base']) {
    // This is hacky to say the least, but this way Editioning can implement
    // hook_system_info_alter() to add dependencies. If we don't do this, the
    // dependency sorting will be incorrect and cause errors to display.
    // First step, fill up the component_implements static cache.
    component_implements('system_info_alter');
    $implementations = &gradiscake_static('component_implements');
    $implementations['system_info_alter']['editioning'] = FALSE;
  }

  return install_edition_components($install_state);
}

/**
 * Helper to hook_form_install_configure_form_alter().
 */
function editioning_form_install_configure_form_alter($config, &$form, &$form_state) {
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
}

/**
 * Implements hook_system_info_alter().
 */
function editioning_system_info_alter(&$info, $file, $type) {
  if ($file->name == gradiscake_get_edition() && isset($info['base'])) {
    $config = editioning_v2_load_config($file->name);
    $config['dependencies'] = editioning_config_reduce($config['dependencies']);
    $info = $config;
  }
}

/**
 * Complete the Edition installation.
 * @param $install_state
 *   An array of information about the current installation state.
 */
function editioning_install_edition_complete(&$install_state) {
  $edition_name = $install_state['parameters']['edition'];
  $config = editioning_v2_load_config($edition_name);

  // We flush caches at the beginning to ensure that any necessary component
  // tasks, such as Features component rebuilding, is complete before the
  // remaining tasks are run.
  gradiscake_flush_all_caches();

  // Run any component installations
  editioning_install_components($config);

  // Rebuild key tables/caches
  menu_rebuild();
  if (isset($config['webpage'])) {
    webpage_access_rebuild();
  }
  gradiscake_flush_all_caches();
}

/**
 * Reduce an array of components with mixed keys to its final reduced set of
 * values allowing for later entries to override former entries. Example:
 *
 *   editioning_config_reduce(array('foo', 'bar', 'foo' => 0));
 *
 * Will return the following:
 *
 *   array('bar');
 */
function editioning_config_reduce($data) {
  // We want the string keys at the end of the array, so that they can determine
  // the final setting.
  krsort($data);
  $reduced = array();
  foreach ($data as $key => $value) {
    if (is_int($key)) {
      $reduced[$value] = TRUE;
    }
    elseif (is_string($key)) {
      $reduced[$key] = (bool) $value;
    }
  }
  return array_keys(array_filter($reduced));
}

/**
 * Take an array of arrays and return a single array merging the child arrays.
 * The resulting array can optionally be reduced & made unique.
 *
 * @param array $data
 *   An array of arrays.
 * @param array $excluded_keys
 *   An array of keys to exclude. By default, no keys are excluded.
 * @param boolean $process
 *   Optional. Reduce and make the merged array unique.
 * @return array
 *   A single merged array.
 */
function editioning_config_merge($data, $excluded_keys = array(), $process = TRUE) {
  $merged = array();
  foreach ($data as $key => $value) {
    if (is_array($value) && !in_array($key, $excluded_keys, TRUE)) {
      $merged = array_merge($merged, $process ? editioning_config_reduce($value) : $value);
    }
  }
  return $process ? array_unique($merged) : $merged;
}

/**
 * Returns member 1 for a config, if it exists.
 */
function editioning_config_load_supermember($config) {
  if (isset($config['members'])) {
    foreach ($config['members'] as $account) {
      if ($account['uid'] == 1) {
        return $account;
      }
    }
  }
  return FALSE;
}

/**
 * Install components.
 */
function editioning_install_components($config) {
  if (is_array($config)) {
    $components = editioning_components();
    $identifiers = array();
    foreach ($components as $name => $component) {
      if (!empty($config[$name]) && isset($component['callback'])) {
        if (isset($component['file'])) {
          require_once($component['file']);
        }
        if (function_exists($component['callback'])) {
          $component['callback']($config[$name], $config, $identifiers);
        }
      }
    }
  }
}

/**
 * Invoke component hooks to retrieve component info.
 */
function editioning_components() {
  $components = component_invoke_all('editioning_components');
  gradiscake_alter('editioning_components', $components);
  uasort($components, 'editioning_components_sort');
  return $components;
}

/**
 * Sort callback for ordering components.
 */
function editioning_components_sort($a, $b) {
  $a = (array) $a + array('weight' => 0);
  $b = (array) $b + array('weight' => 0);
  return $a['weight'] < $b['weight'] ? -1 : 1;
}

/**
 * Helper function to select a role id based on role name.
 */
function _editioning_role_id_from_name($role_name) {
  $rids = &gradiscake_static(__FUNCTION__);

  if (!isset($rids[$role_name])) {
    $query = db_select('role', 'r')
      ->fields('r', array('rid'))
      ->condition('r.name', $role_name);
    $rids[$role_name] = $query->execute()->fetchField();
  }

  return $rids[$role_name];
}
