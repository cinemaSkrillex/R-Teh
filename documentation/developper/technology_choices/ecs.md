# Technology choices - Entity Component System

### 1. **Why Choose ECS**

- **Separation of Concerns**:  
  - ECS allows clear separation of **data** (components), **logic** (systems), and **entities**, reducing complexity and increasing maintainability.  

- **Scalability**:  
  - ECS is well-suited for handling **large numbers of entities** efficiently by organizing data in **contiguous memory** (cache-friendly).  

- **Performance**:  
  - Optimized for **data locality**, making it highly performant for systems that need to process many entities in parallel.  

- **Flexibility**:  
  - Easy to extend and modify. Adding or removing components or systems is straightforward and doesn't break other parts of the code.  

- **Decoupling**:  
  - Makes it easier to introduce **new systems** without affecting others, allowing for **better modularity** and ease of debugging.

### Alternatives to ECS - Examples

1. **Object-Oriented Programming (OOP)**

   - **Example**: A `Player` class with properties like `health`, `position`, and methods like `move()` or `attack()`. The `Enemy` class may inherit from a base class and share behavior like `takeDamage()`, but often ends up with bloated class hierarchies when adding new features.
   
   - **Why not chosen**: In a large game, having all entities share common behavior via inheritance can make it difficult to manage, extend, or mix different behaviors for entities (e.g., a player who is both an NPC and a boss).

2. **Data-Driven Design**

   - **Example**: Game entities defined in external configuration files or databases. For instance, a `character` might have data such as `health`, `damage`, and `speed`, stored in JSON, and the behavior is driven by external scripts.
   
   - **Why not chosen**: While flexible, it can lead to **separation between data and logic**, making it harder to integrate dynamic game mechanics or behaviors, as the systems need to constantly sync between data and code.

3. **Component-Based Design (non-ECS)**

   - **Example**: A `Position` component and a `Health` component are stored separately, and entities are manually given components at runtime. Each system (like `MovementSystem` or `DamageSystem`) operates over those components, but there is no framework managing the relationships or interactions between them.
   
   - **Why not chosen**: Without an ECS framework, developers need to manage entity-component relationships manually, leading to more boilerplate code and inefficient management as the game grows in complexity.
