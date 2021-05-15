<?php
/**
 * @file
 * System component integration.
 */

/**
 * Implementation of hook_editioning_components() on behalf of system.
 */
function system_editioning_components() {
  return array(
    'files' => array('callback' => 'editioning_install_files'),
    'theme' => array('callback' => 'editioning_install_theme'),
    'variables' => array('callback' => 'editioning_install_variables'),
  );
}

/**
 * Component install callback for 'file'.
 */
function editioning_install_files($files, $config, &$identifiers) {
  foreach ($files as $file) {
    // Ensure file has a uri (source) and destination.
    if (isset($file['uri']) && isset($file['destination'])) {
      if (file_prepare_directory(dirname($file['destination']), FILE_CREATE_DIRECTORY)) {
        // Prepare file object.
        $destination = $file['destination'];
        unset($file['destination']);
        $file = (object) $file;

        $scheme = file_uri_scheme($file->uri);
        switch ($scheme) {
          case 'http':
          case 'https':
            $request = gradiscake_http_request($file->uri);
            if ($request->code == 200) {
              $file = file_save_data($request->data, $destination, FILE_EXISTS_REPLACE);
            }
            break;

          default:
            if (file_exists($file->uri)) {
              $file = file_copy($file, $destination, FILE_EXISTS_REPLACE);
            }
        }

        // Allow other components to modify the File object.
        gradiscake_alter('editioning_install_files_file', $file);
      }
    }
  }
}

/**
 * Component install callback for 'theme'.
 */
function editioning_install_theme($theme_name, $config, &$identifiers) {
  theme_enable(array($theme_name));
  variable_set('theme_default', $theme_name);
}

/**
 * Component install callback for 'variables'.
 */
function editioning_install_variables($vars, $config, &$identifiers) {
  foreach($vars as $key => $value) {
    // Handle custom keys.
    if ($key == 'member_admin_role' && $rid = _editioning_role_id_from_name($value)) {
      $value = $rid;
    }
    variable_set($key, $value);
  }
}
