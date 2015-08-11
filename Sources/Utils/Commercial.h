/*	Copyright © 2015 Lukyanau Maksim

	This file is part of Cross++ Game Engine.

    Cross++ Game Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Cross++ is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Cross++.  If not, see <http://www.gnu.org/licenses/>			*/
	
#pragma once

#include <functional>
#include <list>

using namespace std;

namespace cross {

class Commercial{
public:
	enum Event{
		AD_LOADED,
		AD_LOAD_FAILED,
		PURCHASE_COMPLETE,
		PURCHASE_CANCELED,
		PURCHASE_FAILED
	};
	virtual void DownloadAd() = 0;
	virtual void ShowAd() = 0;
	virtual void Purchase() = 0;
	void RegisterCallback(function<void(Event&)> callback);
	void CommercialResult(Event e);
	virtual ~Commercial() {};
private:
	function<void(Event&)> callback;
};

}
