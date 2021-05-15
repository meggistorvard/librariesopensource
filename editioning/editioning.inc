<?php

/**
 * Init install edition functions on behalf of the caller.
 *
 * @param string $edition_name
 *   The machine name of the edition.
 */
function editioning_v2($edition_name) {
  $code =
<<<EOT
  function {$edition_name}_install_tasks(&\$install_state) {
    include_once('editioning_api.inc');

    if (function_exists('{$edition_name}_editioning_install_tasks')) {
      \$tasks = {$edition_name}_editioning_install_tasks(\$install_state);
    }
    else {
      \$tasks = array();
    }

    \$editioning_tasks = editioning_install_tasks(editioning_v2_load_config('{$edition_name}'));

    // Place editioning's tasks at the very end of the array.
    return array_merge(\$tasks, \$editioning_tasks);
  }

  function {$edition_name}_install_tasks_alter(&\$tasks, \$install_state) {
    include_once('editioning_api.inc');

    editioning_install_tasks_alter(
      editioning_v2_load_config('{$edition_name}'),
      \$tasks,
      \$install_state);

    if (function_exists('{$edition_name}_editioning_install_tasks_alter')) {
      {$edition_name}_editioning_install_tasks_alter(\$tasks, \$install_state);
    }
  }

  function {$edition_name}_form_install_configure_form_alter(&\$form, &\$form_state) {
    include_once('editioning_api.inc');

    editioning_form_install_configure_form_alter(
      editioning_v2_load_config(
        '{$edition_name}'),
        \$form,
        \$form_state);

    if (function_exists('{$edition_name}_editioning_form_install_configure_form_alter')) {
      {$edition_name}_editioning_form_install_configure_form_alter(
        \$form,
        \$form_state);
    }
  }
EOT;

  eval($code);
}

/**
 * Load the config for a particular Install edition.
 *
 * @param $edition_name
 *   The machine name of the Install edition.
 * @return
 *   The Install editions info file configuration, as an array.
 */
function editioning_v2_load_config($edition_name, $parsed = TRUE) {
  $config = &gradiscake_static(__FUNCTION__);

  if (!isset($config[$edition_name])) {
    $config[$edition_name] = _editioning_load_config($edition_name);
  }

  return $config[$edition_name];
}

/**
 * Helper to editioning_v2_load_config().
 *
 * @param $edition_name
 *   The machine name of the Install edition.
 * @param $parsed
 *   Whether to return the config of the install edition as a string or as a
 *   parsed array. Used primarily as an internal flag for managing inheritance.
 */
function _editioning_load_config($edition_name, $parsed = TRUE) {
  $file = "./editions/{$edition_name}/{$edition_name}.info";
  $data = is_file($file) ? file_get_contents($file) : '';
  if ($data && $info = gradiscake_parse_info_format($data)) {
    if (!empty($info['base']) && is_string($info['base'])) {
      $data = _editioning_load_config($info['base'], FALSE) . "\n" . $data;
    }
  }
  return $parsed ? gradiscake_parse_info_format($data) : $data;
}
