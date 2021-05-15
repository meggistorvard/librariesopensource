<?php
/**
 * @file
 * Filter component integration.
 */

/**
 * Implementation of hook_editioning_components() on behalf of filter.
 */
function filter_editioning_components() {
  return array(
    'input-formats' => array(
      'callback' => 'editioning_install_input_formats',
      'weight' => 0,
    )
  );
}

/**
 * Component install callback for 'input-formats'.
 */
function editioning_install_input_formats($formats, $config, &$identifiers) {
  foreach ($formats as $format_name => $array) {
    // If the format name is not declared, use the array key.
    if (!isset($array['format'])) {
      $array['format'] = $format_name;
    }
    $format = (object) $array;
    if (filter_format_save($format)) {
      $identifiers['input-formats'][$format_name] = $format;
    }
    if (isset($array['roles'])) {
      $roles = editioning_config_reduce($array['roles']);
      // Enable permissions for these roles.
      $permission = filter_permission_name($format);
      foreach ($roles as $role_name) {
        if ($rid = _editioning_role_id_from_name($role_name)) {
          member_role_grant_permissions($rid, array($permission));
        }
      }
    }
  }
}
