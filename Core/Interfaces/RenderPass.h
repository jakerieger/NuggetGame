//
// Created by conta on 2/26/2024.
//

#pragma once

class IRenderPass {
public:
    virtual ~IRenderPass() = default;
    virtual void Init() {}
    virtual void Render() {}
    virtual void Cleanup() {}

    void SetEnabled(const bool enabled) {
        m_Enabled = enabled;
    }

protected:
    bool m_Enabled = true;
};