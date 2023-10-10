
export const update_player_card = (player, playerData) => {
    player.m_player_card.m_name_element.textContent = playerData.m_name;
    player.m_player_card.m_health_element.textContent = playerData.m_health;
    player.m_player_card.m_armor_element.textContent = playerData.m_armor;
    player.m_player_card.m_kills_element.textContent = playerData.m_stats.kills;
    player.m_player_card.m_deaths_element.textContent = playerData.m_stats.deaths;
    player.m_player_card.m_assists_element.textContent = playerData.m_stats.assists;
};

export const create_player_card = (player, playerData) => {
    // const {m_name_element, m_health_element, m_armor_element} = player.m_player_card;
    
    player.m_player_card = {
        m_parent_element: undefined,
        m_name_element: undefined,
        m_health_element: undefined,
        m_armor_element: undefined,
        m_money_element: undefined,
        m_kills_element: undefined,
        m_deaths_element: undefined,
        m_assists_element: undefined
    };

    // let parentElement = player.m_player_card.m_parent_element;
    let playerCardObject = player.m_player_card;

    if (player.m_player_card.m_parent_element) {
        player.m_player_card.m_parent_element.destroy();
        console.log("destroyed existing player card");
    }

    // Create a new <li> element with class "player-card"
    player.m_player_card.m_parent_element = document.createElement("li");
    player.m_player_card.m_parent_element.className = "player-card";

    // Create the first row within the player card
    const firstRow = document.createElement("div");
    firstRow.className = "player-card-first-row";

    // Create the player name element
    playerCardObject.m_name_element = document.createElement("div");
    playerCardObject.m_name_element.className = "player-card-name";
    console.log(playerData);
    playerCardObject.m_name_element.textContent = "loading";

    // Create the vitals container
    const vitalsContainer = document.createElement("div");
    vitalsContainer.className = "player-card-vitals";

    // Create health vital
    let health = document.createElement("div");
    health.className = "player-card-vital";
    const healthIcon = document.createElement("div");
    healthIcon.className = "icon";
    const healthImage = document.createElement("img");
    healthImage.className = "icon-image";
    healthImage.src = "./assets/icons/health.svg";
    healthIcon.appendChild(healthImage);
    health.appendChild(healthIcon);
    playerCardObject.m_health_element = document.createTextNode("0");
    health.appendChild(playerCardObject.m_health_element);

    // Create armor vital
    let armor = document.createElement("div");
    armor.className = "player-card-vital";
    const armorIcon = document.createElement("div");
    armorIcon.className = "masked-icon";
    const armorImage = document.createElement("img");
    armorImage.className = "icon-image-for-mask";
    armorImage.src = "./assets/equipment/armor.svg";
    armorIcon.appendChild(armorImage);
    armor.appendChild(armorIcon);
    playerCardObject.m_armor_element = document.createTextNode("0");
    armor.appendChild(playerCardObject.m_armor_element);

    // Append player name and vitals container to the first row
    firstRow.appendChild(playerCardObject.m_name_element);
    vitalsContainer.appendChild(health);
    vitalsContainer.appendChild(armor);
    firstRow.appendChild(vitalsContainer);
    player.m_player_card.m_parent_element.appendChild(firstRow);

    // Create the second row within the player card
    const secondRow = document.createElement("div");
    secondRow.className = "player-card-first-row";

    // Create the left column for money and stats
    const leftColumn = document.createElement("div");
    leftColumn.className = "player-card-column";


    // // Create money element
    const moneyElement = document.createElement("div");
    moneyElement.className = "player-card-money";
    let dolarSign = document.createTextNode("$");
    moneyElement.appendChild(dolarSign);
    playerCardObject.m_money_element = document.createTextNode("0");
    moneyElement.appendChild(playerCardObject.m_money_element);
    
    // // Create stats container
    let statsContainer = document.createElement("div");
    statsContainer.className = "player-card-stats";

    // // Create K, D, A stats
    // player.m_player_card.stats.forEach((stat) => {
        
    // });

    for (let index = 0; index <= 2; index++) {
        const statElement = document.createElement("div");
        statElement.className = "player-card-stat";
        const statName = document.createElement("div");
        statName.className = "player-card-statname";
        statName.textContent = index == 0 && "K" || index == 1 && "D" || index == 2 && "A";
        const statValue = document.createElement("div");
        statValue.className = "player-card-statvalue";
        let statValueTextNode = document.createTextNode("0");
        statValue.appendChild(statValueTextNode);
        if (index == 0)
            playerCardObject.m_kills_element = statValueTextNode;
        else if (index == 1)
            playerCardObject.m_deaths_element = statValueTextNode;
        else if (index == 2)
            playerCardObject.m_assists_element = statValueTextNode;

        statElement.appendChild(statName);
        statElement.appendChild(statValue);
        statsContainer.appendChild(statElement);
    }

    leftColumn.appendChild(moneyElement);
    leftColumn.appendChild(statsContainer);
    secondRow.append(leftColumn);
    player.m_player_card.m_parent_element.appendChild(secondRow);

    // // Create the right column for weapons and utilities
    const rightColumn = document.createElement("div");
    rightColumn.className = "player-card-column player-card-column-right";

    // // Create primary weapon element
    // const primaryWeapon = document.createElement("div");
    // primaryWeapon.className = "player-card-weapons-primary";
    // const primaryWeaponImage = document.createElement("img");
    // primaryWeaponImage.className = "player-card-img-primary";
    // primaryWeaponImage.src = "./assets/equipment/ak47.svg";
    // primaryWeapon.appendChild(primaryWeaponImage);

    // // Create secondary weapon element
    // const secondaryWeapon = document.createElement("div");
    // secondaryWeapon.className = "player-card-weapons-secondary";
    // const secondaryWeaponImage = document.createElement("img");
    // secondaryWeaponImage.className = "player-card-img-secondary";
    // secondaryWeaponImage.src = "./assets/equipment/usp_silencer.svg";
    // secondaryWeapon.appendChild(secondaryWeaponImage);

    // // Create utilities container
    // const utilitiesContainer = document.createElement("div");
    // utilitiesContainer.className = "player-card-row-utilities";

    // // Create utility elements (frag grenade in this case)
    // for (let i = 0; i < 5; i++) {
    //     const utility = document.createElement("div");
    //     utility.className = "player-card-weapons-utility";
    //     const utilityImage = document.createElement("img");
    //     utilityImage.className = "player-card-img-utility";
    //     utilityImage.src = "./assets/equipment/frag_grenade.svg";
    //     utility.appendChild(utilityImage);
    //     utilitiesContainer.appendChild(utility);
    // }

    // // Append primary weapon, secondary weapon, and utilities container to the right column
    // rightColumn.appendChild(primaryWeapon);
    // rightColumn.appendChild(secondaryWeapon);
    // rightColumn.appendChild(utilitiesContainer);

    // // Append left column and right column to the second row
    // secondRow.appendChild(leftColumn);
    // secondRow.appendChild(rightColumn);

    // // Append first row and second row to the player card
    // playerCard.appendChild(firstRow);
    // playerCard.appendChild(secondRow);

    return playerCardObject;
    // Append the player card to the desired parent element (e.g., a list)
    // const parentElement = document.querySelector("#your-parent-element"); // Replace with the actual parent element
    // parentElement.appendChild(playerCard);
};
