#include "Input.hpp"

namespace ge = game_engine;

Input::Input() {
    is_inited_ = false;
    previous_interact_state_ = false;
    previous_flashlight_state_ = false;
    last_control_timestamp_ = -1.0;
}

int Input::Init(ge::GameEngine * engine) {
    
    engine_ = engine;

    is_inited_ = true;
    return 0;
}

int Input::Destroy() {

    is_inited_ = false;
    return 0;
}

bool Input::IsInited() {
    return is_inited_;
}

ControlInput_t Input::GetControls() {

    ge::KeyControls_t key_controls = engine_->GetControlsInput();
    if (ge::Equal(key_controls.timestamp_, last_control_timestamp_)) return control_input_;

    last_control_timestamp_ = key_controls.timestamp_;
    control_input_.MOVE_UP_ = key_controls.KEY_ARROW_UP + key_controls.KEY_W;
    control_input_.MOVE_DOWN_ = key_controls.KEY_ARROW_DOWN + key_controls.KEY_S;
    control_input_.MOVE_LEFT_ = key_controls.KEY_ARROW_LEFT + key_controls.KEY_A;
    control_input_.MOVE_RIGHT_ = key_controls.KEY_ARROW_RIGHT + key_controls.KEY_D;
    control_input_.RUN_ = key_controls.KEY_LSHIFT;
    control_input_.QUIT_ = key_controls.KEY_ESC;
    control_input_.ZOOM_IN_ = key_controls.KEY_PAGE_DOWN;
    control_input_.ZOOM_OUT_ = key_controls.KEY_PAGE_UP;
    control_input_.INTERACT_PRESSED = key_controls.KEY_E;

    /* Change flashlight state only when the button F is released */
    if (!key_controls.KEY_F && previous_flashlight_state_) control_input_.FLASHLIGHT_ = !control_input_.FLASHLIGHT_;
    previous_flashlight_state_ = key_controls.KEY_F;

    /* Interact with object should only be done when the E button is released */
    control_input_.INTERACT_ = false;
    if (!key_controls.KEY_E && previous_interact_state_) control_input_.INTERACT_ = true;
    previous_interact_state_ = key_controls.KEY_E;
    
    return control_input_;
}


