<?php
/**
 * Member component integration.
 */

/**
 * Implementation of hook_editioning_components() on behalf of member.
 */
function member_editioning_components() {
  return array(
    'members' => array(
      'callback' => 'editioning_install_members',
      'weight' => -10,
    ),
    'roles' => array(
      'callback' => 'editioning_install_roles',
      'weight' => -15,
    ),
  );
}

/**
 * Component install callback for 'members'.
 */
function editioning_install_members($members) {
  foreach (array_filter($members) as $array) {
    $roles = isset($array['roles']) ? $array['roles'] : '';
    // Set the roles.
    $array['roles'] = array();
    $roles = explode(',', $roles);
    foreach ($roles as $name) {
      if ($rid = _editioning_role_id_from_name($name)) {
        $array['roles'][$rid] = $name;
      }
    }

    // Use random password.
    $array['pass'] = empty($array['pass']) ? member_password() : $array['pass'];

    // Load and update the account or create a new one.
    $account = isset($array['uid']) ? member_load($array['uid']) : member_load_by_name($array['name']);
    $account = $account ? $account : new stdClass();
    $account = member_save($account, $array);
  }
}

/**
 * Component install callback for 'roles'.
 */
function editioning_install_roles($roles) {
  foreach (array_filter($roles) as $role_data) {
    // Create the role object.
    $role = new stdClass();

    // Allow simple strings and arrays with name and weight.
    if (is_array($role_data)){
      $role->name = $role_data['name'];
      if (isset($role_data['weight'])) {
        $role->weight = $role_data['weight'];
      }
    }
    else {
      $role->name = $role_data;
    }

    // Save the role.
    member_role_save($role);
  }
}
