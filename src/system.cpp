#include <algorithm>

#include "system.h"
#include "component.h"
#include <iostream>

namespace mnes {

    unsigned System::GetFlag(int set)                       { return m_flags[set]; }
    std::vector<Entity>& System::GetEntities()              { return m_entities[0]; }
    std::vector<Entity>& System::GetEntities(int set)       { return m_entities[set]; }
    std::unordered_map<int, unsigned>& System::GetSets()    { return m_flags; }

    void System::Require(std::type_index t, int set) {
        if (!m_flags.count(set)) { m_entities[set] = std::vector<Entity>(); }

        const auto c_id = Components::GetId(t);   
        unsigned bit = 1 << c_id;
        m_flags[set] |= bit;
    }

    void System::AddEntity(Entity e, int set) {
        m_entities[set].push_back(e);
    }

    void System::RemoveEntity(Entity e, int set) {
        m_entities[set].erase(std::remove_if(m_entities[set].begin(), m_entities[set].end(), [&e](Entity o) { return e == o; }), m_entities[set].end());
    }


    SystemManager::~SystemManager() {
        m_systems.clear();
    }

    EntityManager& SystemManager::GetEntityManager() const{ return *m_entity_manager; }


    void SystemManager::AddToSystems(Entity e) {
        for (auto& s : m_systems) { 
            for (auto& f : s.second->GetSets()) {
                if (Components::FlagsMatch(f.second, e.GetFlag()) && !Components::FlagsMatch(f.second, e.GetOldFlag())) { s.second->AddEntity(e, f.first); }
                if (!Components::FlagsMatch(f.second, e.GetFlag()) && Components::FlagsMatch(f.second, e.GetOldFlag())) { s.second->RemoveEntity(e, f.first); }
            }
        }
    }

    void SystemManager::RemoveFromSystems(Entity e) {
        for (auto& s : m_systems) { 
            for (auto& f : s.second->GetSets()) {
                if (!Components::FlagsMatch(f.second, e.GetFlag()) && Components::FlagsMatch(f.second, e.GetOldFlag())) { s.second->RemoveEntity(e, f.first); }
            }
        }
    }

    void SystemManager::Init() { 
        for (auto& s : m_systems) { 
            s.second->Init(); 
        } 
    }

    void SystemManager::Update() { 
        for (auto e : m_entity_manager->GetBindableEntities()) { 
            AddToSystems(e); 
            m_entity_manager->UpdateFlags(e); 
        }
        
        for (auto& s : m_systems) { 
            s.second->Update(); 
        } 

        for (auto e : m_entity_manager->GetRemovableEntities()) { 
            RemoveFromSystems(e); 
            m_entity_manager->Remove(e); 
        }
        m_entity_manager->Clear();
    }
};